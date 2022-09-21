#include "jax_custom_call.h"

#include <optional>
#include <pybind11/pybind11.h>
#include "tensorflow/compiler/xla/service/custom_call_target_registry.h"
#include "tensorflow/compiler/xla/service/custom_call_status.h"

struct JaxFFIStatus {
  std::optional<std::string> message;
};

int JAX_FFI_V1 = 1;

int JaxFFIVersionFn() {
  return JAX_FFI_V1;
}

void JaxFFIStatusSetSuccessFn(JaxFFIStatus* status) {
  status->message = std::nullopt;
}

void JaxFFIStatusSetFailureFn(JaxFFIStatus* status, const char* message) {
  status->message = std::string(message);
}

void *JaxFFI_API_Table[] = {
  (void *)&JaxFFIVersionFn,
  (void *)&JaxFFIStatusSetSuccessFn,
  (void *)&JaxFFIStatusSetFailureFn
};

namespace jax {

std::optional<std::string> JaxFFIStatusGetMessage(JaxFFIStatus* status) {
  return status->message;
}

PYBIND11_MODULE(_jax_custom_call, m) {
}

void JaxFFICallWrapper(void* output, void** inputs, XlaCustomCallStatus* status) {
}

XLA_CPU_REGISTER_CUSTOM_CALL_TARGET_WITH_SYM("jax_ffi_call",
                                             &JaxFFICallWrapper);

}
