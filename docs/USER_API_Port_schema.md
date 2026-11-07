# Port schema metadata

Normal blocks export the declarations of their reflected ports in
`BlockModel::metaInformation()["gr:port_schema"]`. Reading this metadata does not
load dynamic port wrappers or start the block. Rebuild plugins against the core
headers to obtain the schema; its presence is not implied by a block name.

Version 1 is a property map with `version` (uint32), `inputs` and `outputs`.
Each direction is a property map keyed by the exact reflected port name. A port
entry contains:

| Field | Meaning |
| --- | --- |
| `type` | Portable name of the port element type, including for empty collections |
| `collection_kind` | `single`, `fixed`, or `dynamic` |
| `fixed_extent` | uint64 extent, present only for a fixed collection; zero is valid |
| `port_kind` | `stream` or `message` |
| `is_optional` | Whether connection is optional |
| `is_synchronous` | Whether the port participates synchronously in scheduling |

The schema is generated from the block's port descriptors, including descriptor
overrides when exposed names are unique within each direction. Ambiguous override
names omit the schema rather than overwrite a descriptor. Tuple members retain their existing descriptor names. Indexed
collections retain the runtime `name#index` addressing convention. Extent is a
declaration, not the observed size of a default instance. A dynamic collection's
size and its controlling setting are unknown unless explicitly described.

Graph and scheduler blocks with ports managed outside normal reflected member
descriptors do not receive a fabricated schema. Consumers must distinguish an
absent or unsupported schema version from an empty schema. The schema describes
port structure; runtime signal metadata and permission to resize connected ports
are separate contracts.
