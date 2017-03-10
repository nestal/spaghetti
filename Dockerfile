FROM fedora:latest
MAINTAINER [Nestal Wan <me@nestal.net>]
RUN dnf update -y && dnf install -y clang-devel qt5-qtbase-devel qt5-qtsvg-devel boost-devel cmake rpm-build gtest-devel
