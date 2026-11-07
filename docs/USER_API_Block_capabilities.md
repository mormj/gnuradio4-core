# Static block capabilities

A block may declare `using BlockMetadata = gr::BlockMetadata<Attributes...>`.
Each attribute provides `static void exportMetadata(auto&& emit)` and emits a
namespaced key and typed value. Core stores these declarations under
`metaInformation()["gr:capabilities"]`, separately from generated port schema.
Keys must be namespaced and unique. Exporters must describe static interfaces and
must not start transports, run processing, or depend on live session state.

Each capability defines and versions its own descriptor. Core does not know
Studio renderer families or payload protocols. An unsupported descriptor must not
be interpreted by guessing from a block name or parameter names. Consumers retain
ownership of supported implementations, while the block declares which interface
it supplies.

The `studio:visualization` version 1 descriptor used by the first-party Series,
PowerSpectrum, and Waterfall sinks declares a renderer family, payload format,
supported transports, semantic kind, and parameter role mapping. Its routing mode
is `session`: runtime endpoints still come from session stream descriptors. The
current managed stream protocol requires conventional `transport` and `endpoint`
setting names. Other roles, including `seriesCount`, reference exact reflected
settings. This descriptor does not load code or add a data-plane transport.

Consumers validate the version, payload/renderer compatibility, supported
transports, and role references. Existing adapters may serve blocks without a
capability. An advertised unsupported capability must not silently select an older
adapter for the same block ID.
