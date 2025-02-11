# Changelog

All notable changes to this project will be documented in this file. See [commit-and-tag-version](https://github.com/absolute-version/commit-and-tag-version) for commit guidelines.

## 0.44.0 (2023-09-11)


### Features

* Add coverage bazel config and collect-coverage tool

## 0.43.0 (2023-08-15)


### Features

* Allow terraform version to be specified


### Bug Fixes

* Move feat(deps) earlier for precedence over feat

## 0.42.1 (2023-08-14)


### Bug Fixes

* Revert override of /usr/bin/python links in build-amazonlinux2

## 0.42.0 (2023-08-07)


### Features

* Add amazonlinux2 support to convert-docker-to-nitro
* Add bsdmainutils for hexdump
* Add build-amazonlinux2023 image
* Add support for amazonlinux2023 in builder.sh
* Configure python3.9 for python/python3 links
* Remove jdk from build-amazonlinux2 image

### Dependencies

* **deps:** Upgrade amazonlinux2 to 20230719


### Bug Fixes

* Empty bazel-* arg list not an error condition

## 0.41.1 (2023-08-04)


### Bug Fixes

* Remove debug statement

## 0.41.0 (2023-08-03)


### Features

* Create links for lld and ld.lld in build-debian

## 0.40.0 (2023-08-03)


### Features

* Add Dependencies section for release notes
* **deps:** Upgrade rules_python to 0.24.0
* Ensure bazel-* scripts handle non-bazel args too

## 0.39.0 (2023-08-02)


### Features

* Run bazel containers in seccomp=unconfined mode

## 0.38.1 (2023-08-02)


### Bug Fixes

* Ensure python3.9 is found first in PATH

## 0.38.0 (2023-07-28)


### Features

* Add cbuild flag --seccomp-unconfined

## 0.37.0 (2023-07-27)


### Features

* Add patch tool to build-debian image

## 0.36.1 (2023-07-25)


### Bug Fixes

* Rename convert_docker_to_nitro to convert-docker-to-nitro

## 0.36.0 (2023-07-24)


### Features

* Add convert_docker_to_nitro

## 0.35.0 (2023-07-21)


### Features

* Add LICENSE file

## 0.34.0 (2023-07-21)


### Features

* Add OSSF Scorecard badge to top-level README

## 0.33.0 (2023-07-20)


### Features

* Install python libclang 15 in build-debian
* Add OSSF Scorecard GitHub Action

## 0.32.0 (2023-07-14)


### Features

* Set PYTHON_BIN_PATH/PYTHON_LIB_PATH in build-amazonlinux2

## 0.31.0 (2023-07-12)


### Features

* Add cbuild --docker-network flag
* Add coverage-tool image plus lcov scripts
* Mount gcloud config dir into container


### Bug Fixes

* Add hash for coverage-tools
* Add hash for coverage-tools
* Improve error handling for flag values
* Print build log path on error condition
* Specify latest image tag explicitly
* Upgrade pre-commit hooks

## 0.30.1 (2023-06-27)


### Bug Fixes

* Use = for --env flag
* Use = for --env flag for all tools

## 0.30.0 (2023-06-26)


### Features

* Install numpy for python3.9
* Set PYTHON_BIN_PATH/PYTHON_LIB_PATH in build-debian
* Upgrade AmazonLinux2 to 20230530
* Upgrade packer to v1.9.1


### Bug Fixes

* Add links for llvm-{cov,profdata}

## 0.29.0 (2023-06-05)


### Features

* Update pre-commit hook versions


### Bug Fixes

* Catch error when shifting multiple args
* Remove golang from test-tools image
* Resolve WORKSPACE using realpath
* Use correct exit code in --fast mode

## 0.28.0 (2023-05-24)


### Features

* Update ca-certificates


### Bug Fixes

* Downgrade to clang v15
* Use builders version.txt for tarfile tag

## 0.27.0 (2023-05-23)


### Features

* Add buf to presubmit image


### Documentation

* Add CONTRIBUTING.md

## 0.26.0 (2023-05-16)


### Features

* Remove zlib-dev package
* Upgrade clang to v16
* Upgrade go to v1.20.4

## 0.25.0 (2023-05-11)


### Features

* Update default bazel version to 5.4.1
* Upgrade rules_python to 0.21.0


### Bug Fixes

* Add file utility to build-debian image

## 0.24.0 (2023-05-05)


### Features

* Add --build-images flag to tests/run-tests
* Reuse tar image if available


### Bug Fixes

* Address linter warnings
* Address linter warnings for tools
* Correct mangled usage text
* Pin pre-commit to 3.x
* Remove .gz suffix from tar file
* Remove function keyword for busybox sh script
* Remove Release in changelog title
* Upgrade pre-commit hooks

## 0.23.0 (2023-04-13)


### Features

* Add wrapper for commit-and-tag-version
* Upgrade curl to version 8
* Upgrade to amazonlinux 2.0.20230320.0


### Bug Fixes

* Use commit-and-tag-version wrapper

## 0.22.0 (2023-04-03)


### Features

* Add awscurl wrapper script
* Add tests for misc CLI wrappers
* Correctly quote bash args
* Extend test-tool to support the release image
* Use login shell for interactive container


### Documentation

* Add section on tools to README
* Remove section on building images directly

## 0.21.1 (2023-03-07)


### Bug Fixes

* Relax pinned version for apache2-utils

## 0.21.0 (2023-03-06)


### Features

* Add wrapper scripts for utils

## 0.20.0 (2023-03-01)


### Features

* Add docker buildkit plugin
* Add tests for CLI wrapper scripts
* Permit testing of a single image
* Relax pinned versions in build-debian, presubmit and test-tools

## 0.19.0 (2023-03-01)


### Features

* Add jq wrapper


### Bug Fixes

* Relax pinned version of openjdk to 11.0.*

## 0.18.0 (2023-02-23)


### Features

* Relax pinned versions for apk and yum packages to semver

## 0.17.0 (2023-02-21)


### Features

* Upgrade black to 23.1.0
* Upgrade tar to 1.34-r1
* Upgrade to buildozer 6.0.1


### Bug Fixes

* Minor code cleanup in images/presubmit/install_apps
* Upgrade ghz to 0.114.0

## 0.16.0 (2023-02-05)


### Features

* Run test tools in docker interactive mode to admit std streams

## 0.15.1 (2023-02-04)


### Bug Fixes

* Return value from get_docker_workspace_mount()

## 0.15.0 (2023-02-03)


### Features

* Use WORKSPACE_MOUNT if set


### Bug Fixes

* Pin commit-and-tag-version to v10.1.0

## 0.14.0 (2023-01-27)


### Features

* Improve verbose output for get-builder-image-tagged

## 0.13.1 (2023-01-26)


### Bug Fixes

* Upgrade software-properties-common

## 0.13.0 (2023-01-23)


### Features

* Add ab tool
* Add cassowary http load testing tool
* Add h2load tool
* Add slowhttptest tool
* Adjust get-builder-image-tagged verbose output
* Upgrade to packer v1.8.5


### Bug Fixes

* Re-pin dependencies in test-tools image
* Relax version pins to semver
* Upgrade amazonlinux2 base image
* Upgrade git on amazonlinux2

## 0.12.0 (2023-01-10)


### Features

* Modify ghz wrapper for generic use. Add curl
* Use test-tools image for grpcurl

## 0.11.0 (2023-01-09)


### Features

* Add chrpath


### Bug Fixes

* Clean up tmpdir via RETURN trap

## 0.10.0 (2023-01-06)


### Features

* Drop ubuntu package version minor for curl

## 0.9.0 (2023-01-04)


### Features

* Add zlib1g-dev to build-debian per scp build dependency
* Update hook versions


### Bug Fixes

* Correct non-zero error message and drop sourcing of tools/builder.sh
* Elide warnings from bazel info
* Revert from clang-format v15 to v14

## 0.8.0 (2022-12-29)


### Features

* Add bash and jq to test-tools image
* Add script to normalize bazel- symlinks
* Ensure run-tests includes all images
* Skip symlinks that resolve in normalize-bazel-symlink

## 0.7.0 (2022-12-27)


### Features

* Add ghz wrapper script
* Add test-tools image

## 0.6.0 (2022-12-12)


### Features

* Add EXTRA_DOCKER_RUN_ARGS support in aws-cli


### Bug Fixes

* Emit docker build output only on non-zero exit
* Remove tempfile before exiting

## 0.5.0 (2022-12-06)


### Features

* Pin versions in presubmit image


### Bug Fixes

* Avoid cache for tar image
* Update version pin for ca-certificates
* Use images subdirs for image list

## 0.4.4 (2022-11-18)


### Bug Fixes

* Retain execute permissions when normalizing dist

## 0.4.3 (2022-11-17)


### Bug Fixes

* Add gettext package for envsubst
* Avoid yum package version strings specific to CPU architecture
* Improve verbose output for get-builder-image-tagged
* Pin apt and yum package versions

## 0.4.2 (2022-11-17)


### Bug Fixes

* Generate SHA within docker container

## 0.4.1 (2022-11-15)


### Bug Fixes

* Reduce noise creating presubmit image
* Remove docker run --interactive flag

## 0.4.0 (2022-11-14)


### Features

* Add buildozer to release image
* Add grpcurl helper script
* Substitute variables in EXTRA_DOCKER_RUN_ARGS
* Use specific version of GNU tar


### Bug Fixes

* Add /opt/bin/python link
* Add CC=clang env var
* Add xz to build-debian
* Explicitly add machine type and OS release to toolchains hash

## 0.3.1 (2022-11-01)


### Bug Fixes

* Add OpenJDK 11 in build-amazonlinux2

## 0.3.0 (2022-11-01)


### Features

* Add git to build-amazonlinux2 image
* Add google-java-format pre-commit hook
* Add OpenJDK 11
* Move python3 to /opt/bin/python3


### Bug Fixes

* Ensure builder::set_workspace does not overwrite WORKSPACE

## 0.2.0 (2022-10-26)


### Features

* Add bazel support to register container-based python toolchain
* Add docker uri env var as override
* Add tools/terraform
* Use image etc files in workspace root


### Bug Fixes

* Avoid empty tar error if no workspace etc files


### Documentation

* Add Getting Started section to README.md

## 0.1.0 (2022-10-25)


### Features

* Add --env flag to cbuild
* Add arg processing to cbuild script
* Add aws-cli helper script
* Add bazel-debian helper script
* Add builders/utils docker image
* Add hadolint to lint Dockerfiles
* Add optional flags to cbuild tool
* Add preliminary support for commit-and-tag-version and copybara
* Add the release-please tool
* Add toolchain short hash to bazel output_user_root path
* Add tools/lib/builder.sh
* **build:** Add GitHub CLI tool https://cli.github.com/
* Determine workspace mount point from docker inspect if inside docker container
* Inject clang-version as a bazel action_env
* Migrate generation of builders/release container image to Dockerfile
* Move image directories to images/ top-level directory
* Overhaul building on amazonlinux2
* Remove python build dependencies from bazel
* Set BUILD_ARCH env var in docker images
* Update release image to node v18
* Upgrade to bazel 5.3.2
* Upgrade to clang v14 on bazel-debian
* Use Packer to build AMI.


### Bug Fixes

* Add builders/tools/normalize-dist to chmod/chgrp/chown dist/ directory tree
* Add get_workspace_mount function to encapsulate code block
* Add python version to action_env
* Add/remove basic pre-commit hooks
* Adopt shellcheck
* Avoid installing recommended debian packages
* Avoid use of git rev-parse to determine tools path
* Bump to latest version of bazelisk
* Clean bazel_root for smaller docker image
* Correct argument handling in cbuild script
* Correct errors generating Amazon Linux 2-based builder image
* Define python3 toolchain
* Drop packer from build-debian image
* Ensure /etc/gitconfig is readable by all
* Improve cbuild help text
* Install bazel version as specified in .bazelversion
* Invoke addlicense for all text files
* Modifications as indicated by shellcheck
* Mount $HOME/aws in aws-cli container
* Move bazel env vars from comments to help text
* Move builder-related configs to builders/etc
* Move WORKSPACE definition to cbuild script global
* Only propagate AWS env vars into amazonlinux2 build container
* Pin version of bazelisk
* Pin version of libc++-dev
* Pin version of python3.8
* Print pre-commit version rather than help
* Remove container when get-architecture exits
* Remove debugging statement
* Remove dockerfile linter ignore and correct ENTRYPOINT
* Remove pre-commit config from build-debian
* Remove shellcheck from build-debian
* Remove unused nitro_enclave_image bazel rule
* Set bazel output_base to accommodate distinct workspaces
* Set bazel output_user_root in image bazelrc
* Set locale in build-debian
* Set WORKSPACE correctly from submodule
* Set WORKSPACE variable
* Switch from hardcoded arch to using dpkg --print-architecture
* Update normalize-dist to function inside build container
* Update pre-commit to use cbuild
* Use PRE_COMMIT_TOOL env var
* Various path-related fixes


### Build System

* Add arch to docker image tags
* Add get_builder_image_tagged tool to determine a content-based tag
* Add get-architecture helper script
* Add missing imports into nitro BUILD
* Add tools/pre-commit
* Correct propagation of quoted args in gh wrapper
* Move commit-and-tag-version into tools dir
* Move gh into tools dir
* Optionally build the AMI
* Propagate status code in exit functions
* Reduce redundant installation commands
* Remove release-please tool
* Rename builders/bazel to build-debian
* Simplify use of npm image in container_run_and_commit()
* Support GH_TOKEN env var


### Documentation

* Add top-level README.md
* Improve prose in README.md
* Move docker build instructions to README.md
* Reformat and lint markdown
