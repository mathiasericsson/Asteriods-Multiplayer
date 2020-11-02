To see older comitt history, check the FPS-Multiplayer-tutorial repository.


The game: MP-Asteriods (but not shot asteroids but each others, like a battle royal :-p)

Goals: 
v Get a client to join a server with sockets
v Build my own client that can be controlled with inputs and join server getting the user ID
v Build a client where I control a "ship" with Asteroids like-controls
v Have the server authorative on the ships position
v Show all connected ships positions on each client
- Create a test enviroment with multiple clients with inputs
- Introduce fake lag
- Implement prediction and correction on client
- Refactor to an ECS architecture


Example code: https://github.com/alaminut/SFML-Space-shooter
Network examples: https://www.codersblock.org/blog/multiplayer-fps-part-1



****** ECS thoughts **********
Entities
- Asteriods
- Ships
- Projectiles
- TextBox (for showing score?)


Components
- Object (->movingObject)
	- positionX
	- positionY
	- rotation
	- velocityX
	- velocityY


- Ship (or should I have a color component and a mesh component?)
	- Color
	- Mesh

- Asteriod
	- Mesh (always same color)

- Helth
	- MaxHealth
	- CurrentHealth

- Projectile
	- Damage


Systems
- Input
- Health (was anything dagamed or destoyed?)
- Position
- Rendering?
- Networking?
- Score?


In practice this would mean...
A ship would have these components
- Object
- Ship
- Health

While an asteroid would have
- Object
- Asteroid

And a "bullet"
- Object
- Projectile





