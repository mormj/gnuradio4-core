# Declared port counts

An integer setting may declare the dynamic port collections whose sizes it
determines:

```cpp
Annotated<gr::Size_t, "channels", Visible, Limits<0U, 32U>,
          PortCountFor<"inputs", "outputs">> channels = 0;
```

Names refer to effective reflected port descriptors, including overrides.
Compilation refuses missing targets, fixed or single ports, non-integer count
settings, and multiple declarations owning the same target. The block still
initializes and resizes the collections. The annotation neither resizes ports nor
grants permission to change a connected or running graph.

Settings initialization exports `channels::port_count_for` as a string list in
`metaInformation()`. Numeric annotated limits export `channels::minimum` and
`channels::maximum`. Equal limit endpoints disable the range check and therefore
export neither bound. Custom validators remain authoritative in C++; these bounds
do not describe additional restrictions imposed by a validator.

The member name, rather than its description string, is the setting reference.
Clients must use exact names and evaluated parameter values, preserve zero, and
leave invalid or unresolved counts unresolved. Without an explicit relationship,
a collection has no known count owner. Observing a default instance's vector size
does not declare a relationship.

The settings member metadata accessor changes the descriptor ABI. Plugin ABI
version 11 requires rebuilding plugins and their hosts together. The relationship
export is independent of the structural port schema and uses the existing
settings metadata interface.
