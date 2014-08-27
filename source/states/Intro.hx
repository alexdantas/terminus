package states;

import flixel.FlxState;
import flixel.FlxSprite;
import flixel.FlxG;
import flixel.util.FlxTimer;
import flixel.util.FlxColor;

/**
 * Shows an introduction to the game
 * (kinda like a slideshow of images).
 */
class Intro extends FlxState
{
	// Images that will switch from one to the other
	private var images:Array<FlxSprite>;

	// Countdown to switch images
	private var timer:FlxTimer;

	// Time to wait on each image (in seconds)
	private static inline var DELAY:Float = 1.1;

	override public function create():Void
	{
		// Let's get it on!
		// Note that music is already interrupted when
		// changing states
		#if flash
		FlxG.sound.playMusic("assets/music/intro.mp3", 1, false);
		#else
		FlxG.sound.playMusic("assets/music/intro.ogg", 1, false);
		#end

		// All "slides"
		images = new Array<FlxSprite>();
		images.push(new FlxSprite(0, 0, "assets/gfx/intro/intro01.png"));
		images.push(new FlxSprite(0, 0, "assets/gfx/intro/intro02.png"));
		images.push(new FlxSprite(0, 0, "assets/gfx/intro/intro03.png"));
		images.push(new FlxSprite(0, 0, "assets/gfx/intro/intro04.png"));
		images.push(new FlxSprite(0, 0, "assets/gfx/intro/intro05.png"));
		images.push(new FlxSprite(0, 0, "assets/gfx/intro/intro06.png"));
		images.push(new FlxSprite(0, 0, "assets/gfx/intro/intro07.png"));
		images.push(new FlxSprite(0, 0, "assets/gfx/intro/intro08.png"));
		images.push(new FlxSprite(0, 0, "assets/gfx/intro/intro09.png"));
		images.push(new FlxSprite(0, 0, "assets/gfx/intro/intro10.png"));
		images.push(new FlxSprite(0, 0, "assets/gfx/intro/intro11.png"));

		add(images[0]);

		timer = new FlxTimer();
		timer.start(
			DELAY,
			function(timer:FlxTimer):Void
			{
				this.nextImage();
			}
		);
	}

	override public function update():Void
	{
		// Interrupt intro if any of those keys
		// is pressed
		if (FlxG.keys.anyPressed(["SPACE", "ENTER"]))
		{
			timer.cancel();
			this.nextState();
		}
		super.update();
	}

	public function nextImage():Void
	{
		remove(images[0]); // Taking from screen

		images.shift().destroy(); // Removes first element of Array
		                          // and returns it, clearing
		                          // FlxSprite from memory

		if (images.length == 0)
		{
			this.nextState();
			return;
		}

		add(images[0]);
		timer.start(
			DELAY,
			function(timer:FlxTimer):Void
			{
				this.nextImage();
			}
		);
	}

	public function nextState():Void
	{
		// Goto next state, with a sweet fade effect
		FlxG.camera.fade(
			FlxColor.BLACK, DELAY, false,
			function()
			{
				FlxG.switchState(new MainMenu());
			}
		);
	}
}
