import { Stream } from "./stream";
import { EventType } from "./events";

export class Clipboard
{
	public static STREAM : Stream;

	private static INIT : number = 0;

	// ==============================================================

	public static setup(canvas: HTMLElement)
	{
		if(Clipboard.INIT == 0)
		{
			Clipboard.INIT = 1;

			canvas.contentEditable = "true";

			document.body.addEventListener("paste", (e) =>
			{
				var cbc: string = e.clipboardData.getData("Text");
				Clipboard.STREAM.sendEvent(
				{
					type: EventType.ClipboardInput,
					size: cbc.length,
					content: cbc
				});
				e.preventDefault();
			});
			    
			//canvas.addEventListener
		  document.body.addEventListener("copy", (e) =>
		  {
				var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "requestServerClipboard", false);
        xhttp.send();
        
        if(xhttp.status == 200)
        {
          var clipContent = "";
          if(xhttp.responseXML.getElementsByTagName("content")[0])
          {
            var clipContent = xhttp.responseXML.getElementsByTagName("content")[0].childNodes[0].nodeValue;
            e.clipboardData.setData("Text", clipContent);
          }
        }
        e.preventDefault();
		  });
		}
	}
}
