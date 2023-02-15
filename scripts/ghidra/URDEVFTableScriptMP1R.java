// Automatically creates definitions for vtables in ARM code.
//@category Symbol

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.data.*;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.listing.Program;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.pcode.*;
import ghidra.program.model.symbol.Namespace;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.program.model.symbol.SourceType;
import ghidra.util.exception.DuplicateNameException;
import ghidra.util.exception.InvalidInputException;
import ghidra.util.task.TaskMonitor;
import org.apache.commons.lang3.tuple.Pair;

import java.util.*;

/**
 * URDEVFTableScriptClean.
 */
public class URDEVFTableScriptMP1R extends GhidraScript {

    public static final List<Long> SKIP_XREF = List.of();
    public static final List<Long> END_AT = List.of(0x7102073080L);

    @Override
    protected void run() throws Exception {
        doWork(currentProgram);
    }

    private void doWork(Program program) throws Exception {
        Listing listing = program.getListing();
        DataTypeManager dataTypeManager = program.getDataTypeManager();
        ReferenceManager referenceManager = program.getReferenceManager();
        Memory memory = program.getMemory();

        Map<Function, Address> results = new HashMap<>();
        results.put(program.getFunctionManager().getFunctionAt(getAddress(program, 0x7100b0763cL)), //CONSTRUCTOR
                    getAddress(program, 0x71020d3630L)); // VTABLE

        DecompInterface decompiler = createDecompiler(program);

        map_lbl:
        for (Map.Entry<Function, Address> entry : results.entrySet()) {
            Function function = entry.getKey();
            Address addr = entry.getValue();

            Namespace parentNamespace = function.getParentNamespace();
            if (parentNamespace == null) {
                println("\nWARN: Namespace for " + function.getName() + " is null! Skipping...");
                continue;
            }
            CategoryPath categoryPath;
            if (parentNamespace.isGlobal()) {
                //                categoryPath = CategoryPath.ROOT;
                println("\nWARN: Can't create vtable for " + function.getName() + " at root! Skipping...");
                continue;
            } else {
                StringBuilder path = new StringBuilder();
                String[] paths = parentNamespace.getSymbol().getPath();
                for (int i = 1; i < paths.length; i++) {
                    path.append("/").append(paths[i]);
                }
                categoryPath = new CategoryPath(path.toString());
            }

            print("\n");
            print("VFTable @ " + addr + " for " + parentNamespace.getName() + "::" + function.getName() + ":\n");

            Data vfTableData = listing.getDefinedDataAt(addr);
            boolean defineData = vfTableData == null || !(vfTableData.getDataType() instanceof Structure);
            if (!defineData) {
                listing.clearCodeUnits(addr, addr.add(vfTableData.getLength()), true);
            }

            int numEntries = 1;
            Address newAddr = addr.add(8);
            while (!END_AT.contains(newAddr.getOffset()) && (!referenceManager.hasReferencesTo(newAddr) || SKIP_XREF.contains(
                    newAddr.getOffset()))) {
                numEntries++;
                newAddr = newAddr.add(8);
            }

            List<Pair<DataType, String>> dataTypes = new ArrayList<>();

            long[] vfTable = new long[numEntries];
            memory.getLongs(addr, vfTable);
            for (int i = 0; i < vfTable.length; i++) {
                if (defineData) {
                    Address vfAddr = addr.add(i * 8);
                    Data dataAt = listing.getDataAt(vfAddr);
                    if (dataAt != null && dataAt.isDefined()) {
                        print("(DEFINED AS " + dataAt.getMnemonicString() + ") ");
                        listing.clearCodeUnits(vfAddr, vfAddr.add(8), true);
                    } else {
                        //                        listing.createData(vfAddr, dataTypeManager.getPointer(DataType.DEFAULT), 8);
                    }
                }

                long addrI = vfTable[i];
                if (addrI == 0) {
                    print("NULL\n");
                    dataTypes.add(Pair.of(dataTypeManager.getPointer(DataType.DEFAULT, 8), null));
                    continue;
                }

                Address fnAddr = getAddress(program, addrI);
                Function functionAt = listing.getFunctionAt(fnAddr);
                if (functionAt == null) {
                    print("(CREATE) " + fnAddr + " => ");
                    functionAt = createFunction(fnAddr, null);
                } else {
                    print(functionAt.getName() + " => ");
                }

                analyzeFunction(decompiler, functionAt, monitor);

                // migrate from old names
                String oldDataTypeName = "vtable__" + functionAt.getName();
                DataType oldDataType = dataTypeManager.getDataType(oldDataTypeName);
                if (oldDataType != null) {
                    dataTypeManager.remove(oldDataType, monitor);
                }

                FunctionDefinitionDataType functionDefinitionDataType = new FunctionDefinitionDataType(functionAt.getSignature());
                DataType returnType = functionDefinitionDataType.getReturnType();
                if (returnType instanceof FloatDataType) {
                    functionDefinitionDataType.setReturnType(new DoubleDataType());
                }
                final ParameterDefinition[] arguments = functionDefinitionDataType.getArguments();
                if (arguments.length >= 1 && arguments[0].getName().equals(
                        "__return_storage_ptr__") && !(returnType instanceof PointerDataType)) {
                    // hack around broken RVO handling in function definitions
                    functionDefinitionDataType.setReturnType(new PointerDataType(returnType));
                }
                for (ParameterDefinition definition : arguments) {
                    if (definition.getDataType() instanceof FloatDataType) {
                        // hack around broken float handling in function definitions
                        definition.setDataType(new DoubleDataType());
                    }
                }

                String dataTypeName = "vtable__" + functionDefinitionDataType.getPrototypeString();
                DataType fnDataType = dataTypeManager.getDataType(dataTypeName);
                if (fnDataType == null) {
                    functionDefinitionDataType.setNameAndCategory(categoryPath, dataTypeName);
                    print(functionDefinitionDataType.getPrototypeString() + "\n");
                    dataTypeManager.addDataType(functionDefinitionDataType, DataTypeConflictHandler.REPLACE_HANDLER);
                } else if (!(fnDataType instanceof FunctionDefinitionDataType)) {
                    throw new RuntimeException("Mismatched data type for " + dataTypeName + ": " + fnDataType.getClass().getName());
                } else {
                    functionDefinitionDataType = (FunctionDefinitionDataType) fnDataType;
                }
                dataTypes.add(Pair.of(dataTypeManager.getPointer(functionDefinitionDataType), functionAt.getName()));
            }
            print("\n");

            //            DataType vfTableStruct = dataTypeManager.getDataType(categoryPath, "vtable");
            StructureDataType structureDataType = new StructureDataType(categoryPath, "vtable", 0, dataTypeManager);
            dataTypes.forEach(p -> {
                final DataType dt = p.getLeft();
                if (dt instanceof PointerDataType) {
                    String name = p.getRight();
                    if (name != null) {
                        name = name.replaceFirst("^vtable__", "");
                    }
                    structureDataType.add(dt, name, null);
                }
            });
            dataTypeManager.addDataType(structureDataType, DataTypeConflictHandler.REPLACE_HANDLER);

            listing.createData(addr, structureDataType);
            program.getSymbolTable().createLabel(addr, "vtable", parentNamespace, SourceType.USER_DEFINED);

            DataType classDt = moveToCategoryPath(dataTypeManager, categoryPath, parentNamespace.getName());
            Structure classStruct;
            if (classDt == null) {
                //                throw new RuntimeException("Failed to find class definition for " + parentNamespace.getName());
                classStruct = new StructureDataType(categoryPath, parentNamespace.getName(), 0, dataTypeManager);
            } else if (!(classDt instanceof Structure)) {
                throw new RuntimeException("Mismatched data type for " + classDt.getName() + ": " + classDt.getClass().getName());
            } else {
                classStruct = (Structure) classDt;
            }
            DataTypeComponent[] existingComponents = classStruct.getComponents();
            for (int i = 0; i < existingComponents.length; i++) {
                DataTypeComponent existingComponent = existingComponents[i];
                final DataType dataType = existingComponent.getDataType();
                if (existingComponent.getOffset() == 0
                        && existingComponent.getLength() == 8
                        && dataType instanceof Undefined8DataType) {
                    classStruct.deleteAtOffset(0);
                    break;
                } else if (existingComponent.getOffset() == 0
                        && existingComponent.getLength() == 8
                        && dataType instanceof Pointer
                        && (((Pointer) dataType).getDataType() instanceof VoidDataType
                        || (((Pointer) dataType).getDataType() instanceof Structure
                        && (((Pointer) dataType).getDataType().getName().equals(
                        "vftable") || ((Pointer) dataType).getDataType().getName().equals("vtable"))))) {
                    classStruct.deleteAtOffset(0);
                    break;
                } else if (existingComponent.getOffset() < 8
                        && existingComponent.getLength() == 1
                        && dataType instanceof DefaultDataType) {
                    classStruct.deleteAtOffset(0);
                    if (i == 3) {
                        break;
                    }
                } else if (existingComponent.getOffset() >= 8) {
                    break;
                } else {
                    printerr(
                            "Found existing component: " + existingComponent.getDataType() + " " + existingComponent.getDataType().getClass());
                    continue map_lbl;
                }
            }
            classStruct.insertAtOffset(0, dataTypeManager.getPointer(structureDataType), 8, "vtable", null);
        }
    }

    private DataType moveToCategoryPath(DataTypeManager dataTypeManager, CategoryPath categoryPath, String dataTypeName) throws DuplicateNameException {
        DataType dataType = dataTypeManager.getDataType(categoryPath, dataTypeName);
        if (dataType != null) {
            return dataType;
        }
        dataType = dataTypeManager.getDataType(CategoryPath.ROOT, dataTypeName);
        if (dataType == null) {
            dataType = dataTypeManager.getDataType(new CategoryPath("/Demangler"), dataTypeName);
        }
        if (dataType != null) {
            dataType.setCategoryPath(categoryPath);
        }
        return dataType;
    }

    private Address getAddress(Program program, Long addr) {
        return program.getAddressFactory().getAddress(Long.toHexString(addr));
    }

    public DecompInterface createDecompiler(Program program) {
        DecompInterface decompiler = new DecompInterface();
        decompiler.toggleCCode(false);
        decompiler.toggleSyntaxTree(false);
        decompiler.setSimplificationStyle("decompile");

        DecompileOptions opts = new DecompileOptions();
        opts.setEliminateUnreachable(false);
        opts.grabFromProgram(program);
        decompiler.setOptions(opts);
        decompiler.openProgram(program);
        return decompiler;
    }

    private void analyzeFunction(DecompInterface decomplib, Function f, TaskMonitor monitor) {
        if (f != null && !f.isThunk() && f.getSignatureSource() == SourceType.DEFAULT) {
            if (!f.isExternal()) {
                if (!this.funcIsExternalGlue(f)) {
                    try {
                        DecompileResults decompRes = null;
                        if (monitor.isCancelled()) {
                            return;
                        }

                        decompRes = decomplib.decompileFunction(f, 10, monitor);
                        String errorMessage = decompRes.getErrorMessage();
                        if (errorMessage != null && !errorMessage.isBlank()) {
                            println(errorMessage);
                        }
                        if (monitor.isCancelled()) {
                            return;
                        }

                        boolean goodInfo = false;
                        if (decompRes.decompileCompleted()) {
                            if (this.hasInconsistentResults(decompRes)) {
                                println("Error function " + decompRes.getFunction() + " has inconsistent parameters");
                                return;
                            }

                            HighFunction hfunc = decompRes.getHighFunction();
                            if (hfunc == null) {
                                return;
                            }

                            HighFunctionDBUtil.commitParamsToDatabase(hfunc, true, SourceType.ANALYSIS);
                            HighFunctionDBUtil.commitReturnToDatabase(hfunc, SourceType.ANALYSIS);

                            goodInfo = true;

                            this.checkModelNameConsistency(decompRes.getFunction());
                        }

                        if (!monitor.isCancelled() && !goodInfo) {
                            String var10001 = f.getName();
                            println("Failed to decompile function: " + var10001);
                        }
                    } catch (Exception var9) {
                        if (!monitor.isCancelled()) {
                            String errMsg = var9.getMessage();
                            if (errMsg == null) {
                                errMsg = "Error decompiling function: " + var9;
                            }

                            println(errMsg);
                        }
                    }
                }
            }
        }
    }

    private boolean funcIsExternalGlue(Function func) {
        String blockName = func.getProgram().getMemory().getBlock(func.getEntryPoint()).getName();
        return blockName.equals("EXTERNAL") || blockName.equals(".plt") || blockName.equals("__stub_helper");
    }

    private boolean hasInconsistentResults(DecompileResults decompRes) {
        HighFunction hfunc = decompRes.getHighFunction();
        if (hfunc == null) {
            return false;
        } else {
            Iterator<HighSymbol> symIter = hfunc.getLocalSymbolMap().getSymbols();

            HighSymbol sym;
            HighVariable highVar;
            do {
                if (!symIter.hasNext()) {
                    return false;
                }

                sym = symIter.next();
                highVar = sym.getHighVariable();
            } while (!(highVar instanceof HighLocal) || !sym.getName().startsWith("in_") || sym.getName().equals(
                    "in_FS_OFFSET") /*|| !sym.getHighVariable().getStorage().isRegisterStorage()*/);

            Function func = hfunc.getFunction();
            if (func != null) {
                String name = func.getName();
                println("Problem recovering parameters in function " + name + " at " + func.getEntryPoint() + " unknown input variable " + sym.getName());
            }

            return true;
        }
    }

    private void checkModelNameConsistency(Function func) {
        int paramCount = func.getParameterCount();
        String modelName = func.getCallingConventionName();
        if (func.getStackPurgeSize() == 0 && paramCount > 0 && modelName.equals("__stdcall")) {
            try {
                func.setCallingConvention("__cdecl");
            } catch (InvalidInputException var5) {
                println("Invalid Calling Convention __cdecl : " + var5);
            }
        }
    }
}
