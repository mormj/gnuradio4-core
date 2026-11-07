#include <boost/ut.hpp>
#include <gnuradio-4.0/BlockModel.hpp>

namespace {
struct ExampleCapability {
    static void exportMetadata(auto&& emit) {
        emit("example:visualization", gr::property_map{{"version", std::uint32_t(1)}, {"payload", "example-v1"}});
    }
};

struct DuplicateCapabilities : gr::Block<DuplicateCapabilities> {
    using BlockMetadata = gr::BlockMetadata<ExampleCapability, ExampleCapability>;
    gr::PortIn<float> in;
    gr::PortOut<float> out;
    GR_MAKE_REFLECTABLE(DuplicateCapabilities, in, out);
    float processOne(float value) const noexcept { return value; }
};

struct SchemaBlock : gr::Block<SchemaBlock> {
    using BlockMetadata = gr::BlockMetadata<ExampleCapability>;
    std::vector<gr::PortIn<double, gr::Async, gr::Optional>> empty;
    std::array<gr::PortIn<float>, 2> fixed;
    std::array<gr::PortOut<float>, 0> zero;
    std::tuple<gr::PortOut<float>, gr::PortOut<double>> tuple;
    gr::MsgPortIn messages;
    GR_MAKE_REFLECTABLE(SchemaBlock, empty, fixed, zero, tuple, messages);
    gr::work::Status processBulk(auto&&...) { return gr::work::Status::OK; }
};

struct AmbiguousSchemaBlock : gr::Block<AmbiguousSchemaBlock> {
    gr::PortIn<float> first;
    gr::PortIn<double> second;
    using OverridePortList = gr::meta::typelist<
        gr::PortIn<float>::make_port_descriptor<"duplicate", gr::detail::SinglePort, 0, 0>,
        gr::PortIn<double>::make_port_descriptor<"duplicate", gr::detail::SinglePort, 0, 1>>;
    GR_MAKE_REFLECTABLE(AmbiguousSchemaBlock, first, second);
    gr::work::Status processBulk(auto&&...) { return gr::work::Status::OK; }
};

const gr::property_map& mapAt(const gr::property_map& map, std::string_view key) {
    return *map.at(std::pmr::string(key)).get_if<gr::property_map>();
}

const boost::ut::suite schemaTests = [] {
    using namespace boost::ut;
    "ambiguous overridden names do not overwrite schema facts"_test = [] {
        AmbiguousSchemaBlock block;
        expect(!block.meta_information.value.contains("gr:port_schema"));
    };
    "schema describes declarations without materializing ports"_test = [] {
        gr::BlockWrapper<SchemaBlock> block;
        const auto& capability = mapAt(mapAt(block.metaInformation(), "gr:capabilities"), "example:visualization");
        expect(eq(capability.at("payload").value_or(std::string_view{}), std::string_view("example-v1")));
        expect(throws<std::invalid_argument>([] { DuplicateCapabilities duplicate; }));
        const auto& schema = mapAt(block.metaInformation(), "gr:port_schema");
        expect(eq(schema.at("version").value_or(0U), 1U));
        const auto& inputs = mapAt(schema, "inputs");
        const auto& empty = mapAt(inputs, "empty");
        expect(eq(empty.at("type").value_or(std::string_view{}), std::string_view("float64")));
        expect(eq(empty.at("collection_kind").value_or(std::string_view{}), std::string_view("dynamic")));
        expect(empty.at("is_optional").value_or(false));
        expect(!empty.at("is_synchronous").value_or(true));
        expect(eq(mapAt(inputs, "fixed").at("fixed_extent").value_or(std::uint64_t(99)), std::uint64_t(2)));
        expect(eq(mapAt(inputs, "messages").at("port_kind").value_or(std::string_view{}), std::string_view("message")));
        const auto& outputs = mapAt(schema, "outputs");
        expect(eq(mapAt(outputs, "zero").at("fixed_extent").value_or(std::uint64_t(99)), std::uint64_t(0)));
        expect(outputs.contains("tuple0") && outputs.contains("tuple1"));
        expect(eq(block.blockRef().empty.size(), 0UZ));
    };
};
}

int main() {}
