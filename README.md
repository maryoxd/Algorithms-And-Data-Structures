# 🗺 Algorithms And Data Structures

**Austria Region Analyzer** is a <b>console-based C++ application</b> developed as a semester project for the <i>Algorithms and Data Structures 1</i> course.<br>
It processes and analyzes population data of Austrian territorial units, builds a multi-level region hierarchy, and provides advanced filtering and sorting operations.

---

## 📚 Features

- <b>Hierarchical Navigation</b> – Traverse between levels like <code>GEO → REPUBLIKA → REGION → OBEC</code>.<br>
- <b>Advanced Filtering</b> – Filter municipalities based on:
  - name match<br>
  - minimum or maximum population<br>
  - type of territorial unit<br>
- <b>Sorting Options</b> – Sort filtered results by:
  - name (German alphabet order)<br>
  - population (male, female, total)<br>
- <b>Custom Predicates</b> – Use lambda-based predicates for reusable filtering logic.<br>
- <b>Dynamic Hierarchy Building</b> – Automatically organizes units into a tree using coded relationships.<br>
- <b>Interactive CLI</b> – Navigate and interact with the dataset through a command-line interface.<br>

---

## 🧠 Key Components

- <code>UzemnaJednotka</code> – Represents a territorial unit with population data across years.<br>
- <code>Loader</code> – Loads and parses .csv input files and creates the hierarchy.<br>
- <code>Algoritmus</code> + <code>Predicates</code> – Generic filtering with custom lambdas.<br>
- <code>SortAlgoritmus</code> – Wrapper around QuickSort using configurable comparators.<br>
- <code>HierarchyNavigator</code> – Enables tree traversal (parent/children).<br>
- <code>Main</code> – CLI-driven application logic controller.<br>

---

## 📊 Technologies & Structures Used

- <b>C++17</b><br>
- <code>std::vector</code> for dynamic sequences<br>
- <code>ImplicitList</code>, <code>SortedSequenceTable</code>, <code>ImplicitSequence</code>, <code>MultiWayExplicitHierarchy</code> (from DS lib)<br>
- Custom lambda expressions and template algorithms<br>
- Manual parsing of <code>.csv</code> files encoded in <b>Windows-1250</b><br>

---
