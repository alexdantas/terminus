package states;

import flixel.FlxG;
import flixel.FlxSprite;
import flixel.FlxState;
import flixel.util.FlxTimer;
import flixel.util.FlxColor;

/**
 * First state, shows a splash screen.
 */
class Splash extends FlxState
{
	// Countdown to switch to next game state
	private var timer:FlxTimer;

	// Time to wait (in seconds)
	private static inline var DELAY:Float = 2;

	// Image to show on the background
	private var background:FlxSprite;

	override public function create():Void
	{
		background = new FlxSprite(0, 0, "assets/gfx/splash-screen.png");
		add(background);

		// Countdown to the
		timer = new FlxTimer(
			DELAY, // Seconds to go
			function(timer:FlxTimer):Void {
				this.nextState();
			}
		);

		super.create();
	}

	override public function update():Void
	{
		// Interrupt splash if any of those keys
		// is pressed
		if (FlxG.keys.anyPressed(["SPACE", "ENTER"]))
		{
			timer.cancel();
			this.nextState();
		}
		super.update();
	}

	public function nextState():Void
	{
		FlxG.camera.fade(
			FlxColor.BLACK, 0.33, false,
			function()
			{
				FlxG.switchState(new Intro());
			}
		);
	}
}
