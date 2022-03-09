package  {
	
	import flash.display.*;
	import flash.text.*;
	import flash.text.*;
	import flash.geom.*;
	import flash.display.*;
	
	public class uiMenu extends MovieClip {
		
		var uiMenu_Buttonx:uiMenu_Button;
		
		public function uiMenu(v_container:DisplayObjectContainer) {
			
			this.x = MainClass.m_stage.stageWidth / 2 - this.width / 2;
			this.y = MainClass.m_stage.stageHeight / 2 - this.height / 2;
			
			var embeddedFont:Font = new kgHappy();
			
			var g_scoreFont:TextFormat;
			g_scoreFont= new TextFormat();
			g_scoreFont.font = embeddedFont.fontName;
			g_scoreFont.size = 45;
			//g_scoreFont.color = 0xF39117
			this["title_txt"].width =  this.width / 2;
	
			this["title_txt"].x = (this.width / 2) - (this["title_txt"].width * 0.9);
			this["title_txt"].y = (this.width * 0.05) - 45 / 2;
			//this["title_txt"].width = this.width / 2;
			//this.b
			
			//g_scoreCounterMC = tmp_moveClip;
			//g_scoreCounterLoaded = true;
			
			this["title_txt"].text = "Menu";
			this["title_txt"].setTextFormat(g_scoreFont);
			//this.embedFonts = true;
			uiMenu_Buttonx = new uiMenu_Button();
			uiMenu_Buttonx.x = this.x + this.width * 0.2; 
			uiMenu_Buttonx.width = this.width * 0.6; 
			uiMenu_Buttonx.y = this.y + this.height * 0.4;
			
			
			//uiMenu_Buttonx.y = (this.width * 0.05) - uiMenu_Buttonx.height / 2;
			
			v_container.addChild(uiMenu_Buttonx);
			//v_container.swapChildren(this, uiMenu_Buttonx);
			//applyTint();
		}
	}
	
}
