#include <gnuradio-4.0/Block.hpp>

struct Conditional : gr::Block<Conditional> {
#if defined(BAD_TYPE)
    int source = 1;
#elif defined(BAD_CYCLE)
    gr::Annotated<bool, "source", gr::EnabledWhen<"first", true>> source = true;
#else
    bool source = true;
#endif
#if defined(BAD_SOURCE)
    gr::Annotated<bool, "first", gr::EnabledWhen<"missing", true>> first = false;
#else
    gr::Annotated<bool, "first", gr::EnabledWhen<"source", true>> first = false;
#endif
    GR_MAKE_REFLECTABLE(Conditional, source, first);
    gr::work::Status processBulk(auto&&...) { return gr::work::Status::OK; }
};

int main() { Conditional block; block.settings().init(); }
