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
import flixel.addons.ui.FlxUICursor;

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

		// Enable a cursor so we can
		// select menu items with the keyboard
		_makeCursor = true;

		super.create();

		// And make it move with the following keys
		cursor.loadGraphic("assets/gfx/ui/cursor.png");
		cursor.setDefaultKeys(
			FlxUICursor.KEYS_DEFAULT_ARROWS |
			FlxUICursor.KEYS_DEFAULT_WASD   |
			FlxUICursor.KEYS_DEFAULT_TAB
		);

		#if flash
		FlxG.sound.playMusic("assets/music/main_menu.mp3", 1, false);
		#else
		FlxG.sound.playMusic("assets/music/main_menu.ogg", 1, false);
		#end
	}

	override public function destroy():Void
	{
		FlxG.sound.music.stop();
		super.destroy();
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

				case "help":
					FlxG.switchState(new Help());

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
