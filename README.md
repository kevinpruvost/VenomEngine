
<p align="center">
  <img src="docs/icons/VenomEngineBanner.png" alt="Venom Engine Logo" width="600">
</p>

<p align="center">
  <a href="#bulb-description">:bulb: Description</a> &nbsp;&bull;&nbsp;
  <a href="#sparkles-showcase">:sparkles: Showcase</a> &nbsp;&bull;&nbsp;
  <a href="#rocket-quick-setup--compilation">:rocket: Quick Setup & Compilation</a> &nbsp;&bull;&nbsp;
  <a href="#classical_building-features--roadmap">:classical_building: Features & Roadmap</a> &nbsp;&bull;&nbsp;
  <a href="#book-documentation--user-guide">:book: Documentation & User Guide</a> &nbsp;&bull;&nbsp;
  <a href="#fountain_pen-license">:fountain_pen: License</a>
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

- [x] Vulkan Renderer
- [ ] Metal Renderer
- [ ] DirectX12 Renderer
- [x] CMake Build System
- [x] HLSL to SPIR-V Compilation
- [x] Ruby Task Runner
- [x] HDR
- [x] Disney Principled BSDF
- [x] Editing GUI
- [x] ECS
- [ ] Custom Renderer Debuggers
- [ ] Protobuf Serialization
- [ ] Virtual Shadow Mapping
- [ ] ...

## :book: Documentation & User Guide

Looking for detailed information? Explore the comprehensive documentation and user guide for `Venom Engine`:

[![View Documentation](https://img.shields.io/badge/View-Documentation-blue?style=for-the-badge&logo=read-the-docs)](https://kevinpruvost.github.io/VenomEngine/html/index.html)

## :fountain_pen: License

[![CC BY-NC-SA 4.0][cc-by-nc-sa-shield]][cc-by-nc-sa]

This work is licensed under a
[Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License][cc-by-nc-sa].

**Dual Licensing:**  
Exclusive dual licensing options are available for commercial or other use cases that do not fit within the terms of the CC BY-NC-SA 4.0 license. If you require different licensing terms, please contact me directly to discuss your needs by [mail](mailto:pruvostkevin0@gmail.com), [Linkedin](https://www.linkedin.com/in/kevin-pruvost-3766a4178) or [Discord](https://www.discordapp.com/users/186896052656406528).

[![CC BY-NC-SA 4.0][cc-by-nc-sa-image]][cc-by-nc-sa]

[cc-by-nc-sa]: http://creativecommons.org/licenses/by-nc-sa/4.0/
[cc-by-nc-sa-image]: https://licensebuttons.net/l/by-nc-sa/4.0/88x31.png
[cc-by-nc-sa-shield]: https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg
