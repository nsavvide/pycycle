<a id="top"></a>

Welcome to the **PyCycle** repository! This project is a blazing-fast, C-based static analysis CLI for detecting circular imports in Python projects.

<div align="center">
  <h1>PyCycle</h1>
  <h3>The fastest way to find circular dependencies in Python</h3>
  <a href="https://github.com/nsavvide/pycycle"><strong>Explore the docs »</strong></a>
  <br /><br />
  <a href="https://github.com/nsavvide/pycycle/issues/new?assignees=nsavvide&labels=bug&projects=&template=1-bug.yml&title=%5BBug%5D%3A+">Report Bug</a>
  ·
  <a href="https://github.com/nsavvide/pycycle/issues/new?assignees=nsavvide&labels=enhancement&template=2-feature-request.yml&title=%5BFeature%5D%3A+">Request Feature</a>
  ·
  <a href="#contact">Contact</a>
</div>

<br />

<div align="center">

[![Contributors][contributors-badge]][contributors-url]
[![Forks][forks-badge]][forks-url]
[![Stargazers][stars-badge]][stars-url]
[![Issues][issues-badge]][issues-url]
[![MIT License][license-badge]][license-url]

</div>

---

### Table of Contents

- [About the Project](#about-the-project)
  - [Built With](#built-with)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
  - [Basic Analysis](#basic-analysis)
  - [Graphviz Export](#graphviz-export)
- [Under the Hood](#under-the-hood)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

## About the Project

Circular imports are one of the most frustrating errors in Python development. Traditional Python-based tools (like those relying on the internal `ast` module) are often slow, memory-heavy, and can fail entirely when encountering complex `__init__.py` package structures. 

**PyCycle** solves this by stepping completely outside the Python ecosystem. Built from scratch in raw C, it uses high-speed lexical tokenization and Graph Theory (DFS) to map your project's architecture and catch cycles instantly.

**Key Features:**
- **Blazing Fast:** Parses thousands of files in milliseconds without booting a Python interpreter.
- **AST-Independent:** Immune to Python package resolution bugs caused by empty `__init__.py` files.
- **Smart Lexer:** Natively understands and resolves:
  - Standard multiline imports (`import os, sys, math`)
  - Aliases (`import django.db as db`)
  - Wildcards (`from utils import *`)
  - Complex Relative Imports (`from ..models import User`)
- **Visualizer:** Exports your entire project architecture to a Graphviz `.dot` file.

<p align="right">
  (<a href="#top">Back to top</a>)
</p>

### Built With

[![C][c-badge]][c-url]

## Prerequisites

To compile and run PyCycle, you only need standard C build tools:

- **GCC** or **Clang**
- **Make**
- **Graphviz** (Optional: Only required if you want to render the `--export` .dot files into PNGs)

## Installation

Because PyCycle is written in C, there are no `pip` dependencies or virtual environments to configure.

1. **Clone the repository:**
   ```bash
   git clone https://github.com/nsavvide/pycycle.git
   cd pycycle
   ```

2. **Compile the binary:**
   ```bash
   make clean && make
   ```

3. **(Optional) Add to your PATH:**
   ```bash
   sudo cp pycycle /usr/local/bin/
   ```

<p align="right">
  (<a href="#top">Back to top</a>)
</p>

## Usage

Simply point PyCycle at the root directory of your Python project. It will recursively scan all .py files and trace the dependency graph.

### Basic Analysis

```bash
./pycycle ./my_python_project
```

Expected Output:
```
Starting PyCycle Analysis...
Target Directory: ./my_python_project
Modules Found: 42
Searching for cycles...

CIRCULAR DEPENDENCY DETECTED
--------------------------------
  -> myapp.models        (line [12])
  -> myapp.managers      (line [4])
  -> myapp.models (CLOSED LOOP)
--------------------------------

Analysis complete.
```

### Graphviz Export

PyCycle can export your project's entire dependency structure so you can visualize the architecture.

```bash
# 1. Export the graph to a .dot file
./pycycle ./my_python_project --export architecture.dot

# 2. Render it to a PNG using Graphviz
dot -Tpng architecture.dot -o graph.png
```

<p align="right">
  (<a href="#top">Back to top</a>)
</p>

## Under the Hood

PyCycle is built with a custom memory-safe architecture:

- **djb2 Hashmap:** For O(1) module string lookups.
- **Dynamic Graph Structs:** Adjacency lists capable of storing line numbers alongside node edges.
- **Relative Path Resolver:** A highly optimized string manipulator that simulates Python's module resolution rules natively in C.

<p align="right">
  (<a href="#top">Back to top</a>)
</p>

## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are greatly appreciated.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

Distributed under the MIT License. See LICENSE for more information.

<p align="right">
  (<a href="#top">Back to top</a>)
</p>

## Contact

Niels Savvides - nsavvide.github.io

Project Link: https://github.com/nsavvide/pycycle

<p align="right">
  (<a href="#top">Back to top</a>)
</p>


<!-- MARKDOWN VARIABLES (LINKS, IMAGES) -->
<!-- Documentation: https://www.markdownguide.org/basic-syntax/#reference-style-links -->
<!-- Badges documentation: https://shields.io/badges -->

[contributors-badge]: https://img.shields.io/github/contributors/nsavvide/pycycle?style=for-the-badge
[contributors-url]: https://github.com/nsavvide/pycycle/graphs/contributors
[forks-badge]: https://img.shields.io/github/forks/nsavvide/pycycle?style=for-the-badge
[forks-url]: https://github.com/nsavvide/pycycle/network/members
[stars-badge]: https://img.shields.io/github/stars/nsavvide/pycycle?style=for-the-badge
[stars-url]: https://github.com/nsavvide/pycycle/stargazers
[issues-badge]: https://img.shields.io/github/issues/nsavvide/pycycle?style=for-the-badge
[issues-url]: https://github.com/nsavvide/pycycle/issues
[license-badge]: https://img.shields.io/github/license/nsavvide/pycycle?style=for-the-badge
[license-url]: https://github.com/nsavvide/pycycle/blob/master/LICENSE.txt

<!-- Other ready-made icons can be seen for example here: https://github.com/inttter/md-badges -->

[html-badge]: https://img.shields.io/badge/HTML-%23E34F26.svg?logo=html5&logoColor=white
[html-url]: https://www.w3schools.com/html/
[css-badge]: https://img.shields.io/badge/CSS-1572B6?logo=css3&logoColor=fff
[css-url]: https://www.w3schools.com/css/
[js-badge]: https://img.shields.io/badge/JavaScript-F7DF1E?logo=javascript&logoColor=000
[js-url]: https://www.javascript.com/
[nextjs-badge]: https://img.shields.io/badge/Next.js-black?logo=next.js&logoColor=white
[nextjs-url]: https://nextjs.org/
[nodejs-badge]: https://img.shields.io/badge/Node.js-6DA55F?logo=node.js&logoColor=white
[nodejs-url]: https://nodejs.org/
[reactjs-badge]: https://img.shields.io/badge/React-%2320232a.svg?logo=react&logoColor=%2361DAFB
[reactjs-url]: https://react.dev/
[redux-badge]: https://img.shields.io/badge/Redux-764ABC?logo=redux&logoColor=fff
[redux-url]: https://redux.js.org/
[sass-badge]: https://img.shields.io/badge/Sass-C69?logo=sass&logoColor=fff
[sass-url]: https://sass-lang.com/
[rust-badge]: https://img.shields.io/badge/Rust-%23000000.svg?logo=rust&logoColor=white
[rust-url]: https://www.rust-lang.org/
[c-badge]: https://img.shields.io/badge/C-00599C?logo=c&logoColor=white
[c-url]: https://www.iso.org/standard/74528.html
[go-badge]: https://img.shields.io/badge/Go-%2300ADD8.svg?logo=go&logoColor=white
[go-url]: https://golang.org/
