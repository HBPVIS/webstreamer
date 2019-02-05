import { Stream } from "./stream";
import { EventType, KeyboardAction } from "./events";

export class KeyMapper
{
	// Maps any character to an ascii xdotool usable token
	private static KEY_MAP = {};

	// Key combo trees
	private static MAP = {};

	// Last key pressed to keep track of combos
	// TODO: Improve to use any length combos
	private static lastKeyPressed = '';

	// =====================================================================================

	// Register key sequences which produces a result
	// F.I., firstKey: Alt Gr + 2 = @
	// Most keys dont need any combo, the result is parsed and sent to server
	// using the key map
	public static registerKeySequence(firstKey: string, secondKey: string, result: string)
	{
		if(!KeyMapper.MAP[firstKey])
		{
			KeyMapper.MAP[firstKey] = {};
		}

		KeyMapper.MAP[firstKey][secondKey] = result;
	}

	// Register a key -> xdotool token map
	// F.I, á = aacute
	public static registerKeyMap(origin : string, result : string)
	{
		KeyMapper.KEY_MAP[origin] = result;
	}

	// =====================================================================================

	public static initializeKeymap()
	{
		// Do not send "Dead" keys to server
		KeyMapper.registerKeyMap('Dead', 'undefined');

		// Do not send altgraph (it is used mainly for accessing characters only)
		KeyMapper.registerKeyMap('AltGraph', 'undefined');

		KeyMapper.registerKeyMap('PrintScreen', 'Print');
		KeyMapper.registerKeyMap('PageUp', 'Page_Up');
		KeyMapper.registerKeyMap('PageDown', 'Page_Down');
		KeyMapper.registerKeyMap('NumLock', 'Num_Lock');
		KeyMapper.registerKeyMap('¡', 'exclamdown');
		KeyMapper.registerKeyMap('!', 'exclam');
		KeyMapper.registerKeyMap('%', 'percent');
		KeyMapper.registerKeyMap('º', 'masculine');
		KeyMapper.registerKeyMap('ª', 'ordfeminine');
		KeyMapper.registerKeyMap('/', 'slash');
		KeyMapper.registerKeyMap('\\', 'backslash');
		KeyMapper.registerKeyMap('*', 'asterisk');
		KeyMapper.registerKeyMap('+', 'plus');
		KeyMapper.registerKeyMap('-', 'minus');
		KeyMapper.registerKeyMap(',', 'comma');
		KeyMapper.registerKeyMap('<', 'less');
		KeyMapper.registerKeyMap('>', 'greater');
		KeyMapper.registerKeyMap('\'', 'apostrophe');
		KeyMapper.registerKeyMap('(', 'parenleft');
		KeyMapper.registerKeyMap(')', 'parenright');
		KeyMapper.registerKeyMap(':', 'colon');
		KeyMapper.registerKeyMap(';', 'semicolon');
		KeyMapper.registerKeyMap('=', 'equal');
		KeyMapper.registerKeyMap('@', 'at');
		KeyMapper.registerKeyMap('`', 'grave');
		KeyMapper.registerKeyMap('´', 'acute');
		KeyMapper.registerKeyMap('|', 'bar');
		KeyMapper.registerKeyMap('¿', 'questiondown');
		KeyMapper.registerKeyMap('?', 'question');
		KeyMapper.registerKeyMap('\"', 'quotedbl');
		KeyMapper.registerKeyMap('&', 'ampersand');
		KeyMapper.registerKeyMap('.', 'period');
		KeyMapper.registerKeyMap('{', 'braceleft');
		KeyMapper.registerKeyMap('}', 'braceright');
		KeyMapper.registerKeyMap('[', 'bracketleft');
		KeyMapper.registerKeyMap(']', 'bracketright');
		KeyMapper.registerKeyMap('^', 'asciicircum');
		KeyMapper.registerKeyMap('_', 'underscore');
		KeyMapper.registerKeyMap('#', 'numbersign');
		KeyMapper.registerKeyMap('¨', 'diaeresis');
		KeyMapper.registerKeyMap('Enter', 'KP_Enter');
		KeyMapper.registerKeyMap('ArrowDown', 'Down');
		KeyMapper.registerKeyMap('ArrowLeft', 'Left');
		KeyMapper.registerKeyMap('ArrowUp', 'Up');
		KeyMapper.registerKeyMap('ArrowRight', 'Right');
		KeyMapper.registerKeyMap(' ', 'space');
		KeyMapper.registerKeyMap('Backspace', 'BackSpace');
		KeyMapper.registerKeyMap('á', 'aacute');
		KeyMapper.registerKeyMap('à', 'agrave');
		KeyMapper.registerKeyMap('ä', 'adiaeresis');
		KeyMapper.registerKeyMap('Á', 'Aacute');
		KeyMapper.registerKeyMap('À', 'Agrave');
		KeyMapper.registerKeyMap('Ä', 'Adiaeresis');
		KeyMapper.registerKeyMap('é', 'eacute');
		KeyMapper.registerKeyMap('è', 'egrave');
		KeyMapper.registerKeyMap('ë', 'ediaeresis');
		KeyMapper.registerKeyMap('É', 'Eacute');
		KeyMapper.registerKeyMap('È', 'Egrave');
		KeyMapper.registerKeyMap('Ë', 'Ediaeresis');
		KeyMapper.registerKeyMap('í', 'iacute');
		KeyMapper.registerKeyMap('ì', 'igrave');
		KeyMapper.registerKeyMap('ï', 'idiaeresis');
		KeyMapper.registerKeyMap('Í', 'Iacute');
		KeyMapper.registerKeyMap('Ì', 'Igrave');
		KeyMapper.registerKeyMap('Ï', 'Igrave');
		KeyMapper.registerKeyMap('ó', 'oacute');
		KeyMapper.registerKeyMap('ò', 'ograve');
		KeyMapper.registerKeyMap('ö', 'odiaeresis');
		KeyMapper.registerKeyMap('Ó', 'Oacute');
		KeyMapper.registerKeyMap('Ò', 'Ograve');
		KeyMapper.registerKeyMap('Ö', 'Odiaeresis');
		KeyMapper.registerKeyMap('ú', 'uacute');
		KeyMapper.registerKeyMap('ù', 'ugrave');
		KeyMapper.registerKeyMap('ü', 'udiaeresis');
		KeyMapper.registerKeyMap('Ú', 'Uacute');
		KeyMapper.registerKeyMap('Ù', 'Ugrave');
		KeyMapper.registerKeyMap('Ü', 'Udiaeresis');
		KeyMapper.registerKeyMap('´´', 'acuteacute');
		KeyMapper.registerKeyMap('``', 'gravegrave');
		KeyMapper.registerKeyMap('Shift', 'shift');
		KeyMapper.registerKeyMap('Control', 'control');
	}

	// =====================================================================================

	public static mapKey(origin : string) : string
	{
		var mapped : string = KeyMapper.KEY_MAP[origin];
		if(mapped)
		{
			if(mapped == 'undefined')
			{
				return '';
			}

			return mapped;
		}

		return origin;
	}

	public static onKeyboardEvent(JSKey : string) : string
	{
		// Transform JS keys into XDOTOOL keys
		var key : string = KeyMapper.mapKey(JSKey);
	
/*
		// Check for combos
		if(evt.type == 'keydown' || evt.type == 'keypress')
		{
			// Check for combo
			if(KeyMapper.lastKeyPressed == '')
			{
				if(KeyMapper.MAP[key])
				{
					KeyMapper.lastKeyPressed = key;
				}
			}
			else
			{
				if(KeyMapper.MAP[KeyMapper.lastKeyPressed][key])
				{
					key = KeyMapper.MAP[KeyMapper.lastKeyPressed][key];
			    KeyMapper.lastKeyPressed = '';
				}
				else if(KeyMapper.MAP[key])
				{
					var toReturn = KeyMapper.lastKeyPressed;
					KeyMapper.lastKeyPressed = key;
					return toReturn;
				}
				else
				{
			
					KeyMapper.lastKeyPressed = '';
					return key;
				}
			}
		}
*/
		return key;
	}
}
