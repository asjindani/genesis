# Genesis

Genesis is a powerful genealogy desktop application built in C++ with the Qt framework. It enables users to explore, create, and manage complex family trees — preserving their heritage across generations through intuitive design and robust logic.

## 🌱 Why Genesis?

"Genesis" represents the origin — the beginning of our stories. This app helps users understand where they came from, preserving family connections across generations in a structured, interactive format.

## ✨ Features

- Add individuals with detailed personal information
- Add and manage family units with relationship metadata
- Set a home person to serve as the focal point of the tree
- Visualize the tree for any individual as an interactive graph
- Auto-save to SQLite database to ensure progress is never lost
- Import or export family trees as JSON files for ease of sharing
- Navigate across family members directly (children ↔ parents ↔ spouses)
- Enforce biological constraints (such as preventing circular ancestry)
- Carefully structured dialogs for adding or editing persons and families
- Confirmations before critical edits (such as gender changes that affect parenting roles)

## 🧱 Core Components

### Person

Represents an individual with their unique attributes.

- Fields: `id`, `firstName`, `lastName`, `gender`, `birthDate`
- Associated with multiple birth and married families
- Methods to retrieve:
  - Full name and formatted output
  - Parent, sibling, and descendant relationships
  - Relationship types and visual representation

### Family

Represents a family unit.

- Fields: `father`, `mother`, `children`
- Each child stores relation metadata with each parent
- Methods:
  - Add or remove children
  - Track child-parent relationship types

### ChildRelation

Binds a child to their parents with clear lineage types.

- Relations: Biological, Adoptive, Foster
- Used to validate ancestry and prevent circular dependencies

### Enum

Defines structured enums:

- `Relation`: Biological, Foster, Adoptive
- `SiblingType`: Full, Half, Step
- `Gender`: Male, Female, Unknown

Includes utility:

- `GenderCast`: cast between `Gender` and `std::string`

### Tree

Central hub to manage the genealogy structure.

- Stores and manages all persons and families
- Autosaves any changes to the SQLite database
- Allows import/export of trees in JSON format
- Enforces ancestry rules and integrity constraints
- Allows setting and changing the focal home person
- Prevents recursive ancestry (no one can be their own ancestor)

### Qt Dialogs

- `Dialog`: For creating or editing individuals
- `FamilyDialog`: For creating or editing families
- `PersonSelect`: To select one or multiple people
- `MainWindow`: Main interface to interact with data
- `TreeDialog`: To visualize a family tree graphically

## 📁 Folder Structure

The files listed below have `.h` and/or `.cpp` extensions.

- `Person`: Defines an individual
- `Family`: Defines a family unit
- `Tree`: Manages entire dataset
- `Enum`: Enumerations and utilities
- `ChildRelation`: Child relationship

## 🚀 Getting Started

1. Clone this repository
2. Download [**Qt Creator**](https://www.qt.io/download)
3. Open the project file
4. Build and run the app
5. Start making your tree

## 🛠 Dependencies

- **C++20**
- **Qt 5 or 6**
- **SQLite** (via `QSqlDatabase`)
- **QGraphicsScene** (for visualizing the tree)

## 🔮 Future Enhancements

- GEDCOM file import/export
- Export tree as PDF or image
- Advanced search and filtering
- Role-based permissions for shared editing
- Support for non-traditional family structures

## 📜 License

MIT License

---

Made with ❤️ and 💡 — because every origin story matters.

## 👤 Author

**Ali Shah**

Computer Science student at Institute of Business Administration.

Driven to build software that connects people to back to their roots.

> To understand ourselves, we must understand where we come from.
> 
> Genesis is my humble journey in code to explore our shared origin.