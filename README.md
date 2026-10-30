# FootballClub

This is a C++ Object-Oriented Programming application designed to manage a football club, including adding players and details such as positions assigned with special trainings, stats, stadium, team bus, league or trophies. It can also simulate games using the rand() function from <cstdlib> library that gives each shoot a chance of goal.

Main Object-Oriented-Programming principles are used such as polymorphism and inheritance hierarchy in order to inherit their attributes and methods ("Jucator" is the parent of "Portar" and "Jucator de Camp", the latter is the parent of "Mijlocas" and "Atacant", each having diferent trainings), use of STL for collections of players and trophies, composition for "AutocarEchipa" (a team bus cannot exist without a team) and aggregation for "Stadion" (a stadium can be used without a team), generic template class <Trofee<T>> to manage team silverware, custom exception handling for players age and shirt number, dynamic downcasting and the use of a design pattern such as singleton for league standings. 

I also use an interactive CLI menu for easy navigation between player management, team assignement, training, match simulation and stats.
