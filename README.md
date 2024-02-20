# SystemRaidSource

<p>This is the source code to my Unreal Engine 5 Game Demo "System Raid," which is available for download on Itch.io <a href="https://carterleew.itch.io/system-raid" target="_blank">here</a>.</p>

<h2>Description</h2>
<p>
	My purpose for this project was to create a five minute game demo that demonstrates my proficiency in several key Unreal Engine 5 features. 
</p>
<p>
	This demo began life as a clone of the classroom project "Toon Tanks" from the GameDev.tv online course "Unreal Engine 5 C++ Developer: Learn C++ and Make Video Games," which explains some of the class and module names. However, most of the code is original, and the game is now very different from the coursework.
</p>

<h2>Key Features</h2>
<p>
	Some of the more creative solutions are found in the Tank.cpp and ExplodingComponent.cpp files.
</p>
<p>
	In ExplodingComponent.cpp, the function ApplyDamage (line 59) will apply damage to actors in a radius from the center of the explosion. This damage is delayed so chained explosions do not occur at exactly the same time, which creates a more realistic effect. This component is featured in-game on the exploding barrels.
</p>
<p>
	In Tank.cpp, the function RotateTurretToController (line 212) adjusts the tank's aim to point where the crosshairs make contact with an object, which ensures that the projectile always lands where the player would expect it to.
</p>
<p>
	I also added Grab, Release, and Throw functions (lines 433, 452, and 464) that act a sort of gravity gun that is used to throw exploding barrels, which will explode on impact. This functionality can be used to any actor that is visible to the Grab trace channel.
</p>
<p>
	I am looking into adding more features and refactoring the code in the future, so check the branches for current and past versions of the source.
</p>
<p>
	Regarding copyright: feel free to use code snippets in your project, with the exception that you do not claim it as your own invention.
</p>
