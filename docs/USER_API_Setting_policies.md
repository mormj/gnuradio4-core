# Setting update policies

`UpdatePolicy<SettingUpdatePolicy::Live>` declares support for applying a setting
while running. `Stopped` requires a stopped instance. `Topology` describes a
change to ports or connections that requires configuring the graph again. An
absent policy is unspecified; writability alone does not establish live support.

Settings initialization exports the declared policy as `setting::update_policy`
with the value `live`, `stopped`, or `topology`. `PortCountFor` implies `topology`
unless the setting explicitly declares the stricter stopped-only policy. A count
setting cannot declare `Live`; compilation rejects that combination.

Clients use the policy to choose available actions. Runtime validation and the
block's callback remain authoritative and may still refuse a live update.
`SettingsBase` does not acquire a lifecycle enforcement mechanism through this
annotation. Applications offering live settings APIs must enforce known
stopped-only and topology restrictions before staging their patches. This metadata
does not promise transaction rollback, hardware confirmation, or that a staged
value is already applied.

For example, a transport interval captured during `start()` is stopped-only even
when its backing C++ field is writable. A setting may be declared live only after
its processing and transport paths support the change.
