package {
	
	import flash.display.*;
	import flash.geom.*;
	
	public class Enemy extends MovieClip {
		
		private var m_direction_x:Boolean;
		private var m_direction_y:Boolean;
		
		private var m_movement_x:Number;
		
		private var m_movement_y:Number;
		private var m_movement_y_timer:Number;
		private var m_movement_y_lock:Boolean;
		private var m_movement_y_lock_timer:Number;
		
		private var m_destroy:Boolean;
		
		var m_numberOfFrame:Number;
		
		public function Enemy(v_x:Number, 
							  v_y:Number, 
							  v_width:Number, 
							  v_height:Number,
							  v_direction_x:Boolean,
							  v_movement:Number) 
							  {
			this.x 		= v_x;
			this.y 		= v_y;
			this.width 	= v_width;
			this.height = v_height;
			m_destroy 	= false;
			
			//Randomise symbol
			m_numberOfFrame = Math.floor( Math.random() * 6 );
			
			//Direction
			m_direction_x = v_direction_x;
			this.gotoAndStop("Enemy_" + m_numberOfFrame.toString());
			
			if(!m_direction_x){
				this["Enemy_" + m_numberOfFrame.toString()].gotoAndStop(2);
			}
			
			//Movement
			m_movement_x 			= v_movement;
			m_movement_y 			= Math.floor( Math.random() * 3 ) + 2;
			m_movement_y_lock_timer = Math.floor( Math.random() * 50 ) + 5;
			
			m_movement_y_timer 		= 0;
			m_movement_y_lock 		= false;
		}
		
		private function updatePosition():void {
			if(!m_destroy) {
				if(m_direction_x) {
					this.x += m_movement_x;
					this["Enemy_" + m_numberOfFrame.toString()].gotoAndStop(1);
					if(this.x > MainClass.m_stage.stageWidth + this.width) {
						m_direction_x = false;
						this.y = Math.floor( Math.random() * 400 ) + 100;
					}
				}
				
				if(!m_direction_x) {
					this.x -= m_movement_x;
					this["Enemy_" + m_numberOfFrame.toString()].gotoAndStop(2);
					if(this.x <= -this.width*2) {
						m_direction_x = true;
						this.y = Math.floor( Math.random() * 400 ) + 100;
					}
				}
				
				if(!m_movement_y_lock) {
					if(m_movement_y_timer > m_movement_y_lock_timer) {
						m_movement_y_timer = 0;
						m_movement_y_lock = true;
						m_direction_y = false;
					} else {
						this.y += m_movement_y;
						m_movement_y_timer++;
					}			
				} else {
					if(m_movement_y_timer > m_movement_y_lock_timer) {
						m_movement_y_timer = 0;
						m_movement_y_lock = false;
						m_direction_y = true;
					} else {
						this.y -= m_movement_y;
						m_movement_y_timer++;
					}			
				}				
			}
		}
	
		public function getDirectionX():Boolean { return m_direction_x; };
		public function getDirectionY():Boolean { return m_direction_y; };
		
		public function getMovementX():Number { return m_movement_x; };
		public function getMovementY():Number { return m_movement_y; };
		
		public function update():void { updatePosition(); }
		public function toDestroy(v_toDestroy:Boolean = false):Boolean { return v_toDestroy; }
	}
}
