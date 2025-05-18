# Genesis

**Genesis** is a genealogy desktop application built in C++ using the Qt framework. It allows users to explore and visualize their origins by creating and managing family trees — from ancestors to descendants.

## 🌱 Why Genesis?

"Genesis" represents the origin — the beginning of our stories. This app helps users understand where they came from, preserving family connections across generations in a structured, interactive format.

## ✨ Features

- Add individuals with personal information
- Add family units to organize relationships
- Interactive dialogs to select required data
- SQLite Database integration to autosave progress
- Principle object-oriented design and architecture

## 🧱 Core Components

### Person

Represents an individual.

- Fields: `id`, `firstName`, `lastName`, `gender`, `birthDate`
- Associated with multiple birth and married families
- Methods to retrieve:
  - Full name and formatted output
  - Parent and sibling relationships
  - Descendants

### Family

Represents a family unit.

- Fields: `father`, `mother`, `children`
- Methods:
  - Add or remove children
  - Track child-parent relationship types

### ChildRelation

Links a child to both parents with relationship metadata.

Relationships include:
- Biological
- Adoptive
- Foster

### Enum

Defines structured enums:

- `Relation`: Biological, Foster, Adoptive
- `SiblingType`: Full, Half, Step
- `Gender`: Male, Female, Unknown

Includes a helper `GenderCast` class for converting gender values.

### Tree

Central hub to manage the genealogy structure.

- Stores all persons and families
- Adds/deletes persons and families
- Can set a home person (focal point)
- Creates required tables in database
- Autosaves changes into the database
- Retrieves data when program reopens

### Qt Dialogs

- `Dialog`: For creating/editing persons
- `FamilyDialog`: For building/editing families
- `PersonSelect`: To select one or multiple people
- `MainWindow`: Main GUI interface to interact with data

## 📁 Folder Structure

The files listed below have `.h` and/or `.cpp` extensions.

- `Person`: Person model
- `Family`: Family model and logic
- `Dialog`: UI for adding persons
- `FamilyDialog`: UI for managing families
- `PersonSelect`: UI for person selection
- `Tree`: Manages entire dataset
- `Enum`: Enumerations
- `ChildRelation`: Child relationship struct

## 🚀 Getting Started

1. Clone this repository.
2. Download [**Qt Creator**](https://www.qt.io/download).
3. Open project.
4. Build and run.
5. Enjoy the app.

## 🛠 Dependencies

- **C++20**
- **Qt 5 or 6** (tested with Qt Creator)
- **SQLite** (optional, used via `QSqlDatabase`)

## 🔮 Future Enhancements

- Improved visualization
- GEDCOM file import/export
- Additional database support
- Export tree as PDF or image
- Advanced search and filtering

## 📜 License

MIT License

---

Made with ❤️ and 💡 — because every origin story matters.

## 👤 Author

**Ali Shah**

Computer Science student at Institute of Business Administration.

Passionate about using technology for things that matter the most.

> To understand ourselves, we must understand where we come from.
> Genesis is my humble journey in code to explore our shared origin.