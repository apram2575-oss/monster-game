cmake_minimum_required(VERSION 3.22)
project(MonsterGame)

set(CMAKE_CXX_STANDARD 17)

add_executable(main
    main.cpp
    Monster.cpp
    Character.cpp
    Enemy.cpp
    Area.cpp
    GameData.cpp
    EventSystem.cpp
    Battle.cpp
    Item.cpp
    Cave.cpp

)
