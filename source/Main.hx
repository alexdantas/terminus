package;

import flash.display.Sprite;
import flash.display.StageAlign;
import flash.display.StageScaleMode;
import flash.events.Event;
import flash.Lib;
import flixel.FlxGame;
import flixel.FlxState;
import flash.system.System; // System.exit()
import states.Splash;
import states.MainMenu;

class Main extends Sprite
{
	// Width/height of the game in pixels
	// (might be less / more in actual
	//  pixels depending on your zoom).
	var gameWidth:Int  = 800;
	var gameHeight:Int = 600;

	// If -1, zoom is automatically calculated to fit the window
	// dimensions.
	var zoom:Float = -1;

	// How many frames per second the game should run at.
	var framerate:Int = 60;

	// Whether to skip the flixel splash screen that appears in
	// release mode.
	var skipSplash:Bool = true;

	// Whether to start the game in fullscreen on desktop targets
	var startFullscreen:Bool = false;

	// First state that the game starts with.
#if debug
	var initialState:Class<FlxState> = MainMenu;
#else
	var initialState:Class<FlxState> = Splash;
#end

	/**
	 * Forcibly and immediately exits the program.
	 */
	public static function exitGame():Void
	{
		#if flash
		// Flash games cannot exit - of course
		// TODO: Create a blank state or something
		#else
		// I don't know if this is safe at all,
		// but at least its something.
		System.exit(0);
		#end
	}

	/**
	 * Start the program from here.
	 */
	public static function main():Void
	{
		Lib.current.addChild(new Main());
	}

	public function new()
	{
		super();

		if (stage != null)
		{
			init();
		}
		else
		{
			addEventListener(Event.ADDED_TO_STAGE, init);
		}
	}

	private function init(?E:Event):Void
	{
		if (hasEventListener(Event.ADDED_TO_STAGE))
		{
			removeEventListener(Event.ADDED_TO_STAGE, init);
		}

		setupGame();
	}

	private function setupGame():Void
	{
		var stageWidth:Int  = Lib.current.stage.stageWidth;
		var stageHeight:Int = Lib.current.stage.stageHeight;

		if (zoom == -1)
		{
			var ratioX:Float = stageWidth / gameWidth;
			var ratioY:Float = stageHeight / gameHeight;

			zoom = Math.min(ratioX, ratioY);

			gameWidth  = Math.ceil(stageWidth / zoom);
			gameHeight = Math.ceil(stageHeight / zoom);
		}

		// Initializing and loading all game settings
		// (including the language)
		Registry.initialize();

		startFullscreen = Registry.save.data.fullscreen;

		addChild(
			new FlxGame(
				gameWidth, gameHeight,
				initialState,
				zoom,
				framerate, framerate,
				skipSplash,
				startFullscreen
			)
		);
	}
}

