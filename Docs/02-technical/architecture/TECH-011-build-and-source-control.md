---
id: TECH-011
title: Build and Source Control
status: proposed
---

# Build and Source Control

## Build

UnrealBuildTool is authoritative; IDE solution/project files are generated conveniences.

Use scripts that receive the Unreal installation path through environment/configuration rather than hard-coding one developer's machine path.

Recommended environment variable:

```text
UE_ROOT=<path to UE_5.7>
```

## Git

Git LFS tracks binary Unreal packages. Use short branches and LFS locks for shared assets.

## CI

GitHub Free minutes/storage are limited. Initial CI should be narrow:

- Text validation/docs links if cheap.
- C++ build only when runners/toolchain are practical.
- Avoid downloading the full LFS corpus unnecessarily in every workflow.

A local pre-merge build/test routine is required even before CI is comprehensive.
