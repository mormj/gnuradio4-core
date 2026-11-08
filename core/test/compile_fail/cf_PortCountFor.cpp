#include <gnuradio-4.0/BlockModel.hpp>

#if defined(BAD_AMBIGUOUS_DYNAMIC) || defined(BAD_AMBIGUOUS_SINGLE)
struct InvalidCount : gr::Block<InvalidCount> {
    std::vector<gr::PortIn<float>> inputs;
#if defined(BAD_AMBIGUOUS_SINGLE)
    gr::PortOut<float> outputs;
    static constexpr auto kOutputKind = gr::detail::SinglePort;
#else
    std::vector<gr::PortOut<float>> outputs;
    static constexpr auto kOutputKind = gr::detail::DynamicPortCollection;
#endif
    // Descriptor indexes include the reflected members inherited from Block.
    static constexpr auto kFirstPort = gr::refl::data_member_count<gr::Block<InvalidCount>>;
    using OverridePortList = gr::meta::typelist<
        gr::PortIn<float>::make_port_descriptor<"ports", gr::detail::DynamicPortCollection, 0, kFirstPort>,
        gr::PortOut<float>::make_port_descriptor<"ports", kOutputKind, 0, kFirstPort + 1>>;
    gr::Annotated<int, "count", gr::PortCountFor<"ports">> count = 1;
    GR_MAKE_REFLECTABLE(InvalidCount, inputs, outputs, count);
    gr::work::Status processBulk(auto&&...) { return gr::work::Status::OK; }
};
static_assert(gr::refl::data_member_name<InvalidCount, InvalidCount::kFirstPort>.view() == "inputs");
static_assert(gr::refl::data_member_name<InvalidCount, InvalidCount::kFirstPort + 1>.view() == "outputs");
#else

struct InvalidCount : gr::Block<InvalidCount> {
    std::vector<gr::PortIn<float>> inputs;
    std::array<gr::PortOut<float>, 2> fixed;
#if defined(BAD_TYPE)
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
#endif

int main() {
    gr::BlockWrapper<InvalidCount> block;
    block.settings().init();
#if defined(BAD_AMBIGUOUS_DYNAMIC) || defined(BAD_AMBIGUOUS_SINGLE)
    block.blockRef().inputs.resize(1);
#if defined(BAD_AMBIGUOUS_DYNAMIC)
    block.blockRef().outputs.resize(1);
#endif
#endif
    block.initDynamicPorts();
#if defined(BAD_AMBIGUOUS_DYNAMIC) || defined(BAD_AMBIGUOUS_SINGLE)
    if (block.dynamicInputPortsSize(0) != 1 || block.dynamicOutputPortsSize() != 1) return 1;
#if defined(BAD_AMBIGUOUS_DYNAMIC)
    if (block.dynamicOutputPortsSize(0) != 1) return 1;
#else
    if (!std::holds_alternative<gr::DynamicPort>(block.dynamicOutputPorts().at(0))) return 1;
#endif
#endif
}
