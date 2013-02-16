//From http://eloquentjavascript.net/chapter14.html
function makeHttpObject() {
  try {return new XMLHttpRequest();}
  catch (error) {}
  try {return new ActiveXObject("Msxml2.XMLHTTP");}
  catch (error) {}
  try {return new ActiveXObject("Microsoft.XMLHTTP");}
  catch (error) {}

  throw new Error("Could not create HTTP request object.");
}


var tosend = "";
var ClientName = Math.random();

//Blocking request - yuck... It's a total hack.
function blockingrequest( surl )
{
	var request = makeHttpObject();
	request.open("GET", surl, false);
	request.send(null);
 
	if (request.status === 200)
		return request.responseText;
	else
		return "";
}

/*

//This was a try for networking - it didn't work.

function cnlohrdataout( s )
{
	if (typeof pc == 'undefined')
		return;

	pc.com2.send_chars( s );
}

function cnlohrdatain( e )
{
//	el = document.getElementById("ttyS1");
//	el.value += e;	
//	tosend += e;

	for( i = 0; i < e.length; i++ )
	{
		var t = e.charCodeAt( i );
		var h = t.toString(16);
		if (h.length == 1)
			h = "0" + h;
		tosend += h;
	}

}

function gotdata( data )
{
	var seennewline = false;
	var i;

	for( i=0; i < data.length; i+=2 )
		cnlohrdataout( String.fromCharCode( parseInt( data.substr(i,2), 16 ) ) );
}

function checksend()
{
	var request = makeHttpObject();

	if( tosend.length > 0 )
	{
		var atosend = tosend.length;
		if( atosend > 200 )
		{
			atosend = 200;
		}

		var stosend = tosend.substr( 0, atosend );

		request.open("GET", "ToServer?"+ClientName+"%0a"+stosend, true);
		tosend = tosend.substr( atosend );
		request.onreadystatechange=function() { }
		request.send(null);
	}

	setTimeout("checksend();",20);
}

setTimeout("checksend();",1000);

function sendchars()
{
	var request = makeHttpObject();
	request.open("GET", "ToClient?"+ClientName+"%0a", true);

	request.onreadystatechange=function()
	{
		if (request.readyState==4)
		{
			if( request.status!=404)
			{
				var r = request.responseText;
				gotdata(r);
			}

			setTimeout( "sendchars();", 20 );
		}
	}

	request.send(null);

//	print(request.responseText);
}

setTimeout("sendchars();",1000);
*/


//This is how we communicate to/from the AVR's IO Ports

function ioport_write8( addy, value )
{
	blockingrequest( "/d/w1?" + ( "00" + addy.toString(16) ).substr( -2 ) +
		( "00" + value.toString(16) ).substr( -2 ) );
}

function ioport_write16( addy, value )
{
	blockingrequest( "/d/w2?" + ( "00" + addy.toString(16) ).substr( -2 ) +
		( "0000" + value.toString(16) ).substr( -4 ) );
}

function ioport_read8( addy )
{
	var ret = blockingrequest( "/d/r1?" + ( "00" + addy.toString(16) ).substr( -2 ) );
	return parseInt( ret, 16 );
}

function ioport_read16( addy )
{
	var ret = blockingrequest( "/d/r2?" + ( "00" + addy.toString(16) ).substr( -2 ) );
	return parseInt( ret, 16 );
}

