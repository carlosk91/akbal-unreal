# Prompt — First Repository Bootstrap

You are a **senior Unreal Engine build/repository engineer** bootstrapping Akbal. Do not implement gameplay.

Read `AGENTS.md`, governance docs, GTD and `PROJECT_STRUCTURE.md`.

Goal:
1. Normalize the repository so `Akbal.uproject` is at repository root if the team approves that migration.
2. Add `Public/` and `Private/` folders to the `Akbal` module without changing behavior.
3. Add the proposed first-party `Content/_Akbal/` folder structure in the Editor as needed.
4. Configure Git LFS for Unreal binary assets and verify `.gitignore`.
5. Add only essential plugin/module dependencies; do not enable gameplay systems yet.
6. Confirm the Editor target builds.

Before moving/renaming existing Unreal assets, stop and ask. This initial project should have little/no content, so repository flattening is expected to be low risk.

Return the exact migration/build commands and validation results.
