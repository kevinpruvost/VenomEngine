
<p align="center">
  <img src="docs/icons/VenomEngineBanner.png" alt="Venom Engine Logo" width="600">
</p>

<p align="center">
  <a href="#bulb-description">:bulb: Description</a> &nbsp;&bull;&nbsp;
  <a href="#sparkles-showcase">:sparkles: Showcase</a> &nbsp;&bull;&nbsp;
  <a href="#rocket-quick-setup--compilation">:rocket: Quick Setup & Compilation</a> &nbsp;&bull;&nbsp;
  <a href="#classical_building-features--roadmap">:classical_building: Features & Roadmap</a> &nbsp;&bull;&nbsp;
  <a href="#book-documentation--user-guide">:book: Documentation & User Guide</a>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/language-C%2B%2B-blue?style=for-the-badge&logo=c%2B%2B">
  <img src="https://img.shields.io/badge/language-Ruby-red?style=for-the-badge&logo=ruby">
  <img src="https://img.shields.io/badge/language-GLSL-green?style=for-the-badge&logo=opengl">
  <img src="https://img.shields.io/badge/language-HLSL-purple?style=for-the-badge&logo=microsoft">
  <img src="https://img.shields.io/github/last-commit/kevinpruvost/VenomEngine?style=for-the-badge&logo=github">
  <a href="https://kevinpruvost.github.io/VenomEngine/html/index.html">
    <img src="https://img.shields.io/badge/View-Documentation-blue?style=for-the-badge&logo=read-the-docs">
  </a>
</p>

## :sparkles: Showcase

TODO

## :warning: <sub>**Warning: This project is a Work in Progress (WIP).**</sub>

## :bulb: Description

`Venom Engine` is my solo Computer Graphics journey, designed to dive deep into cutting-edge technologies:

- 🧑‍💻️ **Graphics APIs**: Mastering `Vulkan`, `Metal`, and `DirectX12`.
- 🛠️ **Build Systems**: Streamlining development with `Bazel`.
- 🖥️ **Technical Tools**: Exploring `Renderer Debuggers`, `protobuf`, and `HLSL`.
- 💻 **Scripting**: Enhancing workflows with `Ruby` and more.

Join me as I explore these technologies and push the boundaries of what's possible in modern graphics programming.

## :rocket: Quick Setup & Compilation

`VenomEngine` uses a combination of `Bazel` & `Makefile` as its build system (will integrate CMake later). 

Here is how to setup and compile the project:

```bash
make dxc
make compile_shaders
make release_run
```

## :classical_building: Features & Roadmap

- [ ] Vulkan Renderer
- [ ] Metal Renderer
- [ ] DirectX12 Renderer
- [x] Bazel Build System
- [ ] CMake Build System
- [x] HLSL to SPIR-V Compilation
- [x] Ruby Task Runner
- [ ] Custom Renderer Debuggers
- [ ] Protobuf Serialization
- [ ] ...

## :book: Documentation & User Guide

Looking for detailed information? Explore the comprehensive documentation and user guide for `Venom Engine`:

[![View Documentation](https://img.shields.io/badge/View-Documentation-blue?style=for-the-badge&logo=read-the-docs)](https://kevinpruvost.github.io/VenomEngine/html/index.html)
