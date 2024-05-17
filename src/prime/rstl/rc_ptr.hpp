#ifndef _RSTL_RC_PTR
#define _RSTL_RC_PTR

#include "types.h"
#include <atomic>

// rc_ptrs seem to be almost entirely changed since MP1
namespace rstl {

    // this could just be a namespace
    struct rc_ptr_private {
        struct rc_ptr_control_block_base {
            virtual bool release() { return false; }
            virtual void destroy() {}
            virtual ~rc_ptr_control_block_base() = default;
        };

        struct rc_ptr_control_block_null : rc_ptr_control_block_base {
            bool release() override { return false; }
            void destroy() override { }
            ~rc_ptr_control_block_null() override = default;
        };

        template <typename T>
        struct rc_ptr_default_control_block : rc_ptr_control_block_base {
            bool release() override {
                mPtr = nullptr;
                return true;
            }
            void destroy() override {
                if(this) {
                    ~rc_ptr_default_control_block<T>();
                }
            }
            // the dtor for this type makes no sense...

            T* mPtr;
        };

        rc_ptr_control_block_base* mControlBlock; // this isnt right, it seems like this type isnt a pointer, but im not sure how else it'd work...
        std::atomic<int> refCount;

        static rc_ptr_private sNull;
    };

    // this is always a size of 0x10
    template <typename T>
    struct rc_ptr {
        rc_ptr_private *privData;

        virtual ~rc_ptr() {
            privData->refCount--;
            if(privData->refCount <= 0 )
                privData->mControlBlock->destroy();
            delete this;
        }

        rc_ptr() = default;

        rc_ptr(const rc_ptr& other) = default;

        rc_ptr(rc_ptr& other) {
            this->privData = other.privData;
            privData->refCount++;
        }

        rc_ptr(rc_ptr* other) {
            this->privData = other->privData;
            privData->refCount++;
        }

        rc_ptr& operator=(const rc_ptr& other) {
            this->privData = other.privData;
            privData->refCount++;
            return *this;
        }
    };

    template < typename T >
    class ncrc_ptr : public rc_ptr< T > {
    public:
        ncrc_ptr() {}
        ncrc_ptr(T* ptr) : rc_ptr< T >(ptr) {}
        ncrc_ptr(const rc_ptr< T >& other) : rc_ptr< T >(other) {}
    };

    // based off what i can tell, it seems like there are std-like make_ funcs for each control block type
    // the only evidence is this symbol:
    // rstl::ncrc_ptr<CModConSceneNode::SInstanceBufferData>
    //      rstl::make_ncrc_ptr<CModConSceneNode::SInstanceBufferData, NRenderInstanced::SInstanceDataEntry const&,
    //                          rstl::enum_bit_field<EVertexComponent, unsigned int, (EVertexComponent)32> const&, unsigned int const&>
    //                          (NRenderInstanced::SInstanceDataEntry const&, rstl::enum_bit_field<EVertexComponent, unsigned int,
    //                          (EVertexComponent)32> const&, unsigned int const&)
    template <class T, class B, class... Args>
    ncrc_ptr<T> make_ncrc_ptr(B& control_block, Args&&... args);


} // namespace rstl

#endif // _RSTL_RC_PTR