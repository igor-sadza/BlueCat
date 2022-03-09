package {
	
	import flash.display.*;
	import flash.display.*;
	import flashx.textLayout.operations.MoveChildrenOperation;
	import flash.geom.*;
	
	public class Player extends MovieClip {
		
		private var m_target:MovieClip;
		private var m_spawned:Boolean;
		private var m_cloud:Cloud;
		private var m_container:DisplayObjectContainer;
		private var m_timer:Number;
		private var m_timerOfDestroy:Number;
		private var m_enemyManager:EnemyManager;
		private var m_enemyDeath:Enemy_Death;
		
		public function Player(v_container:DisplayObjectContainer, v_enemyManager:EnemyManager) {
			
			m_container = v_container;
			this.width = 75;
			this.height = 100;
			this.visible = false;
			m_spawned = false;
			m_enemyDeath = new Enemy_Death(m_container);
			m_enemyDeath.setSize(this.width*1.25, this.height*1.25);
			
			m_cloud = new Cloud(m_container);
			m_cloud.setSize(this.width*1.25, this.height*1.25);
			m_cloud.setOffsetColor(0,0,139);
			m_timer = 0;
			m_timerOfDestroy = 0;
			m_enemyManager = v_enemyManager;
			m_container.addChild(this);
			
			m_container.swapChildren(this,m_cloud);
			applyTint();
		}
		
		private function checkCollision(object:MovieClip):Boolean {
			var coolisionX:Boolean = this.x >= m_target.x + m_target.width/2 - 10 && m_target.x + m_target.width/2 + 10 >= m_target.x
			var coolisionY:Boolean = this.y >= m_target.y + m_target.height/2 - 10 && m_target.y + m_target.height/2 + 10 >= m_target.y
			return coolisionX && coolisionY;
		}
		
		//private function checkCollision_Y(object:MovieClip):Boolean {
			//var coolisionY:Boolean = this.y >= object.y && object.y + (this.height/2) >= this.y;
			//return coolisionY;
		//}
		
		private function checkCollision_X(object:MovieClip):Boolean {
			var coolisionX:Boolean = this.x >= object.x + (object.width/2) - 30 && object.x + (object.width/2) + 30 >= this.x;
			return coolisionX;
		}
		
		private function hidePlayer() {
			this.visible = false;
		}
		
		private function showPlayer() {
			this.visible = true;
		}
		
		public function setPlayerTarget(v_target:MovieClip) {
			m_target = v_target;			
		}
		
		public function setPlayerPosition(v_pos_x:Number, v_pos_y:Number) {
			this.x = v_pos_x;
			this.y = v_pos_y;
		}
		
		function random(min:int = 0, max:int = int.MAX_VALUE):int
		{
			if (min == max) return min;
			if (min < max) return min + (Math.random() * (max - min + 1));
			else return max + (Math.random() * (min - max + 1));
		}
		
		public function spawnPlayerNearTarget():void {
			if(!m_spawned) {
				if(m_target != null) {
					
					var t_x:Number =  random(m_target.x-200, m_target.x+200);
					var t_y:Number = random(m_target.y-200, m_target.y+200);	
					
					this.x = t_x;
					this.y = t_y;
					
					
					
					m_cloud.setPosition(t_x, t_y);
					m_cloud.playOnce();		
					m_spawned = true;				
				}
			}
		}


		//TODO
		// BETTER COLISSION SYSTEM
		private function followTarget() {
			if(m_spawned && m_target != null && m_cloud.isHalfOfAnimation()) {
				
				//m_container.swapChildren(this,m_target);
				
				//if (!checkCollision_Y(m_target)) {
					if(this.y <= m_target.y + m_target.height/2) {
						this.y += m_target.getMovementY() * 2;
					}
					else {
						this.y -= m_target.getMovementY() * 2;
					}
					
					if (this.y >= m_target.y + m_target.height/2 - 10 && m_target.y + m_target.height/2 + 10 >= m_target.y) {
						this.y = m_target.y + m_target.height/2;
					}
					
				//}
				
				//if(!checkCollision_X(m_target)) {
					if(this.x <= m_target.x + m_target.width/2) {
						this.x += m_target.getMovementX() * 2;
					} else if (this.x >= m_target.x + m_target.width/2) {
						this.x -= m_target.getMovementX() * 2;
					}
					
					//this.x >= object.x + (object.width/2) - 30 && object.x + (object.width/2) + 30 >= this.x;
					if (this.x >= m_target.x + m_target.width/2 - 10 && m_target.x + m_target.width/2 + 10 >= m_target.x) {
						this.x = m_target.x + m_target.width/2;
					}
					

					
				//} 
				//else {
					//if(this.x <= m_target.x) {
						//this.x += m_target.getMovement();
					//} else {
						//this.x -= m_target.getMovement();
					//}
				//}
				
				if(checkCollision(m_target)) {
						if(m_timerOfDestroy > 5) {
								
								Enemy(m_target).toDestroy(true);
								
								m_enemyDeath.setPosition(m_target.x, m_target.y);
								m_enemyDeath.playOnce();
							
								m_enemyManager.deleteEnemy(m_target);
								m_timerOfDestroy = 0;
								this.visible = false;
								m_spawned = false;
								m_timer = 0;
								m_cloud.setPosition(this.x, this.y);
								m_cloud.playOnce();
								
	
								
								m_target = null;		
						} else {
							m_timerOfDestroy++;
						}
				}				
			}
		}
		
		private function hidePlayerAfter() {
			if(m_spawned) {
				if(m_timer > 200) {
					this.visible = false;
					m_spawned = false;
					m_timer = 0;
					m_cloud.setPosition(this.x, this.y);
					m_cloud.playOnce();	
				} else {
					m_timer++;
				}
				
				if(m_cloud.isHalfOfAnimation()) {
					this.visible = true;
				}
			}
		}
		
		public function update() {
			//spawnPlayerNearTarget();
			followTarget();
			hidePlayerAfter();
		}
		
		private function applyTint():void {
			var m_color:ColorTransform = this.transform.colorTransform; 
			m_color.redOffset = 0;
			m_color.greenOffset = 0;
			m_color.blueOffset = 139;
			this.transform.colorTransform = m_color;
		}
	}
	
}
