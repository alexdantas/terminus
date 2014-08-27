package states;

import flixel.FlxG;
import flixel.FlxSprite;
import flixel.FlxState;
import flixel.util.FlxTimer;
import flixel.util.FlxColor;

/**
 * First state, shows a splash screen.
 */
class Help extends FlxState
{
	// Image to show on the background
	private var background:FlxSprite;

	override public function create():Void
	{
		background = new FlxSprite(0, 0, "assets/gfx/bg_help_screen.png");
		add(background);

		super.create();
	}

	override public function update():Void
	{
		// Interrupt splash if any of those keys
		// is pressed
		if (FlxG.keys.anyPressed(["SPACE", "ENTER"]))
			FlxG.switchState(new MainMenu());

		super.update();
	}
}
