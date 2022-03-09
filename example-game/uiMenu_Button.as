package  {
	
    import flash.display.*;
    import flash.display.*;
    import flash.display.*;
	
	public class uiMenu_Button extends MovieClip {
		
		
		public function uiMenu_Button() {
			var defaultImage:uiMenu_Button_png = new uiMenu_Button_png();
			defaultImage.x = 0;
			defaultImage.y = 0;
            var myImage:Bitmap = new Bitmap(defaultImage);
			myImage.x = 0;
			myImage.y = 0;
            addChild(myImage);
		}
	}
	
}
