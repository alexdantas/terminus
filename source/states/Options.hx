package states;

import flixel.FlxG;
import flixel.addons.ui.FlxUIState;
import flixel.addons.ui.FlxUIRadioGroup;
import flixel.addons.ui.FlxUICheckBox;

class Options extends FlxUIState
{
	public override function create():Void
	{
		// Which XML file to use when building UI
		// (must be inside "assets/xml")
		_xml_id = "ui_options_menu";

		super.create();
	}

	public override function getEvent(event:String, target:Dynamic, data:Dynamic, ?params:Array<Dynamic>):Void
	{
		// Which Event just happened?
		switch (event)
		{
		// Right after we load, let's fill the
		// Language Radio with the current language
		case "finish_load":
			var radio:FlxUIRadioGroup = cast _ui.getAsset("locale_radio");
			if (radio != null)
			{
				if (Registry.language != null)
					radio.selectedId = Registry.language.locale.toLowerCase();
			}

			var checkbox:FlxUICheckBox = cast _ui.getAsset("fullscreen_toggle");
			if (checkbox != null)
				checkbox.checked = FlxG.fullscreen;

		case "click_button":
			if (params != null && params.length > 0)
			{
				var command:String = cast params[0];

				if (command == "back")
					FlxG.switchState(new MainMenu());
			}

		case "click_check_box":
			if (params != null && params.length > 0)
			{
				var command:String = cast params[0];

				if (command == "fullscreen")
				{
					FlxG.fullscreen = !FlxG.fullscreen;
					Registry.save.data.fullscreen = FlxG.fullscreen;
					Registry.apply();
				}
			}

		// When selecting the language, immediately
		// change it and reload the state
		case "click_radio_group":
			var id:String = cast data;

			if (Registry.language != null)
			{
				// Restart with other language,
				// calling function when done
				Registry.language.init(id, reloadState);

				Registry.save.data.language = id;
				Registry.apply();
			}
		}
	}

	private function reloadState():Void
	{
		FlxG.switchState(new Options());
	}
}
