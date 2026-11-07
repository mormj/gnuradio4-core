#include <gnuradio-4.0/Block.hpp>

struct InvalidCount : gr::Block<InvalidCount> {
    std::vector<gr::PortIn<float>> inputs;
    std::array<gr::PortOut<float>, 2> fixed;
#if defined(BAD_POLICY)
    gr::Annotated<int, "count", gr::PortCountFor<"inputs">, gr::UpdatePolicy<gr::SettingUpdatePolicy::Live>> count = 1;
#elif defined(BAD_TYPE)
    gr::Annotated<float, "count", gr::PortCountFor<"inputs">> count = 1;
#elif defined(BAD_FIXED)
    gr::Annotated<int, "count", gr::PortCountFor<"fixed">> count = 1;
#elif defined(BAD_TARGET)
    gr::Annotated<int, "count", gr::PortCountFor<"missing">> count = 1;
#else
    gr::Annotated<int, "count", gr::PortCountFor<"inputs">> count = 1;
#endif
#if defined(BAD_OWNER)
    gr::Annotated<int, "other", gr::PortCountFor<"inputs">> other = 1;
#else
    int other = 1;
#endif
    GR_MAKE_REFLECTABLE(InvalidCount, inputs, fixed, count, other);
    gr::work::Status processBulk(auto&&...) { return gr::work::Status::OK; }
};

int main() { InvalidCount block; block.settings().init(); }
