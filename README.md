# ScratchQL [![Build Status](https://travis-ci.com/anarchuser/ScratchQL.svg?branch=master)](https://travis-ci.com/anarchuser/ScratchQL)
#### A relational database management system from scratch
----
### This page is about
* the **goal** of this project
* how to **set up** the project for development
* how to **contribute** to this project
* **testing** your code
* details **about** the project



## Goal
This project aims to build a relational database management system (short RDBMS) from scratch, in c++20.
It should be [ACID](https://en.wikipedia.org/wiki/ACID) and implement basic [CRUD](https://en.wikipedia.org/wiki/Create,_read,_update_and_delete) functionality.
It should Consistent, Available and thus not Partitioned (see [CAP](https://en.wikipedia.org/wiki/CAP_theorem)).
Also, latency shall be minimised at the cost of bandwidth.



## Setup
* Clone this repository
* Make sure the following tools are installed:
  * an ISO C++20-compliant compiler
  * catch2, glog, make, cmake
* Run `bash setup/git-config` to setup git hooks
* We recommend JetBrain’s CLion due to its integrated build automation.
  If you use other IDEs, update the `.gitignore` file accordingly.



## Contribution
* Fork our [GitHub Repo](https://github.com/anarchuser/Database)
* Commit guidelines: see [Karma Runner](https://karma-runner.github.io/4.0/dev/git-commit-msg.html)
* Pushing your changes:
  * Create Pull Requests for changes
  * PR titles should follow the commit guidelines
  * At least one review is required for approval
  * Merge through Squash’n Merge
* All production code belongs in `/src/*` which mirrors
  * our Model-View-Controller architecture
  * our use of our object orientation
    * Single inheritance, subclasses in subfolders



## Testing
* Logging Framework: Glog (Google logging)
* Testing framework: Catch2
* Behaviour-driven development where applicable, otherwise test-driven
* All testing belongs in `/test/*`
* `/test/*` should mirror `/src/*` in its folder structure



## About
Semester Project Spring 2020 at
[CODE University of Applied Sciences](https://code.berlin)

Copyright (C) 2020 Aaron Alef <aaron.alef@code.berlin> [@anarchuser](https://github.com/anarchuser)