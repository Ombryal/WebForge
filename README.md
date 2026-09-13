<div align="center">

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=0:00C6FF,100:6C5CE7&height=220&section=header&text=WebForge&fontSize=75&fontColor=ffffff&animation=fadeIn&fontAlignY=38&desc=Write.%20Compile.%20Forge.%20The%20Web.&descAlignY=58&descSize=20"/>

### ⚡ A modern programming language for building the web

<img src="https://readme-typing-svg.demolab.com?font=Fira+Code&size=21&pause=1200&color=00C6FF&center=true&vCenter=true&width=650&lines=One+file.+One+language.+One+website.;HTML+without+the+boilerplate.;Style%2C+logic+%26+UI+in+one+place.;Components%2C+state+%26+animations.;Forge+the+web+your+way." alt="Typing SVG"/>

<br/>

![Status](https://img.shields.io/badge/status-early%20development-orange?style=for-the-badge)
![C++20](https://img.shields.io/badge/C%2B%2B-20-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.22%2B-064F8C?style=for-the-badge&logo=cmake&logoColor=white)
![License](https://img.shields.io/github/license/<Ombryal>/WebForge?style=for-the-badge)

<br/>

<a href="https://github.com/Ombryal/WebForge">
<img src="https://img.shields.io/badge/⭐_STAR_WEBFORGE-00C6FF?style=for-the-badge&logo=github&logoColor=white&labelColor=151515"/>
</a>

</div>

---

## 🚀 What is WebForge?

WebForge is a small programming language and compiler for building websites from a single `.wf` file.

Instead of manually writing separate HTML, CSS and JavaScript, WebForge aims to provide one language for structure, styling, behaviour, components and more.

```wf
page "Hello World"

heading "Welcome to WebForge"

container {
    text "One file. One language. One website."

    button "Say Hi" {
        on click {
            alert("Hello from WebForge!")
        }
    }
}
```

Compile it:

```bash
./build/webforge examples/hello.wf
```

Into:

```text
hello.wf
   │
   ▼
 WebForge
   │
   ▼
hello.html
```

---

## ✨ Features

### ✅ Current

- Declarative `.wf` syntax
- Lexer → Parser → AST → CodeGen
- HTML generation
- Headings, text, images, links, lists and containers
- Buttons and basic events
- Standalone HTML output
- C++20 + CMake

### 🔮 Planned

- 🎨 CSS & styling
- ⚡ JavaScript generation
- 🧩 Components & reusable layouts
- 🧠 Variables & expressions
- 🔀 Conditionals & loops
- 📱 Responsive layouts
- 🖱️ Interactive states & events
- 🔥 State management
- 🎬 Animations
- 🎞️ Transitions & keyframes
- 📦 Modules & packages
- 🛠️ Formatter & linter
- 💡 Language Server & editor support
- 🔄 Hot reload
- 🌐 Standard library & package ecosystem

---

## 🎬 Future WebForge

The long-term goal is to make styling, animations and interactions first-class language features.

```wf
page "WebForge"

style {
    background "#0b0b12"
    color "#ffffff"

    animation fadeIn {
        duration 500ms
        easing ease_out
    }
}

container class="hero" {
    heading "Forge the Web"

    text "Build modern websites with one language."

    button "Get Started" {
        on click {
            navigate "/docs"
        }
    }
}
```

> [!NOTE]
> The styling and animation syntax above represents the planned direction of WebForge and is not implemented in the current compiler yet.

---

## 🏗️ Architecture

```mermaid
flowchart LR
    A[".wf Source"] --> B["Lexer"]
    B --> C["Parser"]
    C --> D["AST"]
    D --> E["Code Generator"]
    E --> F["HTML / CSS / JS"]
    F --> G["Browser"]
```

---

## 📁 Project Structure

```text
WebForge/
├── src/webforge/
│   ├── lexer/
│   ├── parser/
│   ├── ast/
│   └── codegen/
├── examples/
├── tests/
├── docs/
├── CMakeLists.txt
├── LICENSE
└── README.md
```

---

## 🛠️ Build

```bash
git clone https://github.com/<your-username>/WebForge.git
cd WebForge

cmake -S . -B build
cmake --build build

./build/webforge examples/hello.wf
```

---

## 🛣️ Roadmap

```text
Lexer + Parser
      ↓
HTML Generation
      ↓
CSS + Styling
      ↓
JavaScript + Events
      ↓
Components + State
      ↓
Animations + Transitions
      ↓
Tooling + IDE Support
      ↓
Packages + WebForge Ecosystem
```

---

## 📖 Documentation

See [`docs/language.md`](docs/language.md) for the current language reference.

---

## 🤝 Contributing

WebForge is still in early development and the language may change significantly.

Issues, ideas and pull requests are welcome.

---

## 📜 License

WebForge is released under the **MIT License**.

See [`LICENSE`](LICENSE).

<div align="center">

<br/>

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=0:6C5CE7,100:00C6FF&height=120&section=footer"/>

### ⚡ WebForge

**Forge the web.**

[⭐ Star](https://github.com/<your-username>/WebForge)
&nbsp; • &nbsp;
[🐛 Issues](https://github.com/<your-username>/WebForge/issues)
&nbsp; • &nbsp;
[📖 Docs](docs/language.md)

</div>
