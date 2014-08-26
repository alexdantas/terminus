package states;

import flixel.FlxG;
import flixel.FlxSprite;
import flixel.FlxState;
import flixel.util.FlxTimer;

/**
 * First state, shows a splash screen.
 */
class Splash extends FlxState
{
	// Countdown to switch to next game state
	private var timer:FlxTimer;

	private var background:FlxSprite;

	override public function create():Void
	{
		background = new FlxSprite(0, 0, "assets/gfx/splash-screen.png");
		add(background);

		// Countdown to the
		timer = new FlxTimer(
			2, // Seconds to go
			function(timer:FlxTimer):Void {
				// Switch to the game
			}
		);

		super.create();
	}

	override public function update():Void
	{
		if (FlxG.keys.anyPressed(["SPACE", "ENTER"]))
		{
			timer.cancel();
			// Switch to the game
		}
		super.update();
	}
}
