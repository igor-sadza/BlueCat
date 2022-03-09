package  {
	
	import flash.display.*;
	import flash.display.*;
	import flash.geom.*;
	
	public class Enemy_Death extends MovieClip {
		
		private var m_container:DisplayObjectContainer;
		
		public function Enemy_Death(v_container:DisplayObjectContainer) {
			m_container = v_container;
			this.visible = false;
			m_container.addChild(this)
		}
		
		public function setSize(v_width:Number, v_height:Number):void {
			this.height 	= v_width;
			this.width 		= v_height;
		}
		
		public function setPosition(v_x:Number, v_y:Number):void {
			this.x 			= v_x;
			this.y 			= v_y;
		}
		
		public function playOnce() {
			this.gotoAndPlay(0);
			this.visible = true;
		}

		
	}
	
}
