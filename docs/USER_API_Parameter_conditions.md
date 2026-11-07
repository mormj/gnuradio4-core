# Boolean parameter presentation conditions

`EnabledWhen<"autoscale", false>` declares that a field is relevant when the
current value of the reflected boolean setting `autoscale` is false. It is a
presentation condition, not a validation constraint or a computed value.

Compilation checks that the source exists and is a reflected plain or annotated
boolean setting. Conditions must not form a cycle. Settings initialization emits
`field::enabled_when_parameter` and `field::enabled_when_value` into metadata.
The expected value is a boolean; arbitrary expressions, enum comparisons, and
arithmetic are outside this first contract.

Clients evaluate the rule against resolved parameter values or a runtime settings
snapshot. If the controller is unresolved, the field is disabled. Disabling a
field never clears its authored value, changes a setting, or grants live-edit
permission. Runtime update policy and writability remain separate checks.

Blocks continue validating accepted configurations in their existing callbacks.
General cross-parameter constraints, dependent bounds, and computed-value rules
require a separate contract rather than extending this condition implicitly.
