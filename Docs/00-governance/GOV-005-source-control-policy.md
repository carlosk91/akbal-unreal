---
id: GOV-005
title: Source Control Policy
status: approved
---

# Source Control Policy

## Stack

- GitHub Free
- Git
- Git LFS

GitHub Free currently includes 10 GiB Git LFS storage and 10 GiB monthly LFS bandwidth. Treat this as a hard production constraint until the team changes hosting/budget.

## Branch names

```text
feature/F001-rhythm-conductor
fix/F001-input-drift
content/jaguar-audio-pass
chore/update-docs
spike/quartz-latency
```

## Commit style

Imperative, English, scoped where useful:

```text
feat(ritual): add sync section state model
fix(stun): preserve heartbeat count on ritual re-entry
docs(gdd): clarify corruption failure
```

## LFS

At minimum, track Unreal binary assets:

```text
*.uasset
*.umap
```

Mark merge-hostile shared binary assets `lockable`.

Production source assets such as WAV/FBX/TGA may also use LFS, but raw DAW sessions, raw footage, caches, and disposable exports should not enter GitHub LFS.

## Binary asset protocol

Before editing a shared `.uasset` or `.umap`:

1. Pull/rebase latest `main`.
2. Lock the file with Git LFS if lockable.
3. Tell the other developer what you own.
4. Keep the branch short.
5. Merge promptly.
6. Unlock after merge.

Never resolve a binary conflict by arbitrarily choosing one side without understanding lost work.

## Maps

Prefer map/content partitioning that reduces both developers editing the same map package. Avoid using a single Level Blueprint as the integration point for unrelated systems.

## Excluded generated directories

Never commit:

```text
Binaries/
DerivedDataCache/
Intermediate/
Saved/
.vs/
.idea/  # except intentionally shared project settings if later approved
```

## LFS budget rule

At ~80% of the free LFS quota, stop adding large optional source assets and review storage strategy before the repository becomes blocked.
