package states;
import flixel.addons.ui.FlxUIState;

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

		super.create();
	}
}
