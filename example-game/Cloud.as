package {
	
	import flash.display.*
	import flash.geom.*;
	
	public class Cloud extends MovieClip {
		private var m_totalFrames:Number;
		private var m_container:DisplayObjectContainer;
		
		public function Cloud(v_container:DisplayObjectContainer) {
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
		
		public function isHalfOfAnimation():Boolean {
			
			if(this.currentFrame >=  Math.floor(this.totalFrames / 4)) {
				return true;
			}
			return false;
		}
		
		public function setOffsetColor(v_red:Number, v_green:Number, v_blue:Number):void {
			var m_color:ColorTransform = this.transform.colorTransform;
			m_color.redOffset 	= v_red;
			m_color.greenOffset = v_green;
			m_color.blueOffset 	= v_blue;
			this.transform.colorTransform = m_color;
		}
		
	}
}
