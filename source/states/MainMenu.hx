package states;

import flixel.FlxG;
import flixel.FlxSprite;
import flixel.FlxState;
import flixel.text.FlxText;
import flixel.ui.FlxButton;
import flixel.util.FlxMath;
import flixel.util.FlxColor;
import flixel.util.FlxSave;
import flixel.addons.ui.FlxUIState;
import flixel.addons.ui.interfaces.IFlxUIWidget;

/**
 * The Main menu.
 * 'nuff said.
 */
class MainMenu extends FlxUIState
{
	override public function create():Void
	{
		// Which XML file to use when building UI
		// (must be inside "assets/xml")
		_xml_id = "ui_main_menu";

		// Let's get it on!
		// Note that music is already interrupted when
		// changing states
		if (FlxG.sound.music == null)
		{
			#if flash
			FlxG.sound.playMusic("assets/music/main_menu.mp3", 1, false);
			#else
			FlxG.sound.playMusic("assets/music/main_menu.ogg", 1, false);
			#end
		}

		super.create();
	}

	// Handles events on the UI
	override public function getEvent(name:String, sender:IFlxUIWidget, data:Dynamic, ?params:Array<Dynamic>):Void
	{
		// Which Event just happened?
		switch (name)
		{
		case "click_button":
			if (params != null && params.length > 0)
			{
				// Which button was pressed?
				switch (cast(params[0], String))
				{
				case "start":
					// Fade out to the next state!
					FlxG.camera.fade(
						FlxColor.BLACK, 0.33, false,
						function()
						{
							FlxG.switchState(new Splash());
						}
					);
				case "options":
					FlxG.switchState(new Options());

				case "intro":
					FlxG.camera.fade(
						FlxColor.BLACK, 0.5, false,
							function()
							{
								FlxG.switchState(new Intro());
							}
					);

				case "quit":
					// Fade out to the Desktop
					FlxG.camera.fade(
						FlxColor.BLACK, 0.33, false,
						function()
						{
							Main.exitGame();
						}
					);
				}
			}
		}
	}
}
