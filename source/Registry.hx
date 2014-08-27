package;

import flixel.util.FlxSave;
import firetongue.FireTongue;
import flixel.addons.ui.FlxUIState;

/**
 * Contains data to be used globally on the game.
 * Settings, saves, whatever.
 *
 * It that can be used to store references to objects and
 * other things for quick-access.
 *
 */
class Registry
{
	/**
	 * Language used to translate all in-game texts.
	 */
	public static var language:MyFireTongue = null;

	public static var shipsDestroyed   :Int = 0;
	public static var distanceTraveled :Int = 0;

	/**
	 * Container for data to be preserved between executions.
	 *
	 * Usage:
	 *
	 *     // Saving something
	 *     Registry.save.data.YOUR_VARIABLE = "value";
	 *     Registry.save.data.flush
	 *
	 *     // Getting it back
	 *     // (don't forget to check if it's NULL)
	 *     Registry.save.data.YOUR_VARIABLE
	 *
	 */
	public static var save:FlxSave = null;

	/**
	 * Initializes the save data to default variables.
	 */
	public static function initialize():Void
	{
		if (Registry.save != null)
		{
			Registry.save.erase();
			Registry.save = null; // TODO: perhaps destroy in a better way?
		}

		Registry.save = new FlxSave();

		// Initializes save with unique identifier
		// It's only this identifier that allows to retrieve
		// the same data later!
		Registry.save.bind("SaveSlot0");

		// Setting all default variables

		if (Registry.save.data.locale == null)
			Registry.save.data.locale = "en-US";

		// Setting up the language
		// (Firetongue instance on the `Registry` class)
		if (Registry.language == null)
		{
			Registry.language = new MyFireTongue();
			Registry.language.init(Registry.save.data.locale);

			// All states will use this tongue.
			FlxUIState.static_tongue = Registry.language;
		}

		if (Registry.save.data.fullscreen == null)
			Registry.save.data.fullscreen = false;
	}

	/**
	 * Writes the changes made to the Registry on the disk.
	 *
	 * @note Make sure to do it so settings can be preserved!
	 */
	public static function apply():Void
	{
		if (Registry.save == null)
			return;

		Registry.save.flush();
	}
}

