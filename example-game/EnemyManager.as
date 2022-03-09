package {
	
	import flash.events.*;
	import flash.display.*;
	import flash.display.*;
	import flash.display.*;
	import flash.events.*;
	import flash.events.*;
	import flash.geom.*;
	import flash.text.*;
	
	public class EnemyManager extends MovieClip {
		
		private var m_container:DisplayObjectContainer;
		private var m_enemies:Array = [];
		private var m_numberOfEnemies:Number;	
		private var m_timer:Number;
		private var m_choosen:Boolean;
		public var m_currentEnemy:MovieClip;
		
		public function EnemyManager(v_container:DisplayObjectContainer, v_numberOfEnemies:Number) {
			
			m_container 		= v_container;
			m_timer 			= 0;
			m_choosen 			= false;
			m_currentEnemy 		= null;
			m_numberOfEnemies 	= v_numberOfEnemies;

			createEnemies(v_numberOfEnemies);
		}
		
		public function getEnemies():Array {
			return m_enemies;
		}
		
		public function deleteEnemy(v_enemy:MovieClip):void {
			for (var i = 0; i < m_enemies.length; i++){
				var tmp_enemy = m_enemies[i] as Enemy;
								
				if (tmp_enemy == v_enemy){
					if (v_enemy == m_currentEnemy && m_currentEnemy == tmp_enemy) {
						m_currentEnemy = null;
					}
					m_enemies.splice(i, 1);
					MainClass.m_enemyLayer.removeChild(v_enemy);
				}
			}
		}
		
		private function checkEnemyCoolision(object_0:MovieClip, object_1:MovieClip):Boolean {
			var coolisionX:Boolean = object_0.x + object_0.width >= object_1.x && object_1.x + object_1.width >= object_0.x;
			var coolisionY:Boolean = object_0.y + object_0.height >= object_1.y && object_1.y + object_1.height >= object_0.y;
			return coolisionX && coolisionY;
		}
		
		private function createEnemies(v_numberOfEnemies:Number):void {
			
			//Create enemies and add to array
			while (m_enemies.length < v_numberOfEnemies){
				m_enemies.push(createEnemy());
			}
			
			//Check colision
			for (var i = 0; i < m_enemies.length; i++) {
				for (var j = i+1; j < m_enemies.length; j++) {
					if (checkEnemyCoolision(MovieClip(m_enemies[i]), MovieClip(m_enemies[j]))) {
						MovieClip(m_enemies[i]).y = Math.floor( Math.random() * 600 ) + 110;
					}
				}
			}
			
			for (var k = 0; k < m_enemies.length; k++){
				MainClass.m_enemyLayer.addChild(MovieClip(m_enemies[k]));
			}
		}
		
		private function createEnemy():Enemy {
				var tmp_x:Number;
				var tmp_y:Number;
				var tmp_height:Number;
				var tmp_width:Number;
				var tmp_direction:Boolean;
				var tmp_movement:Number;
				
				//Size
				tmp_width =  Math.floor( Math.random() * 10 ) + 140;
				tmp_height =  Math.floor( Math.random() * 20 ) + 90;
				
				tmp_movement = Math.floor( Math.random() * 3 ) + 2;
				
				//Position
				var rand:Number = Math.floor( Math.random() * 10 ) + 1;
				if (rand % 2) {
					tmp_x = MainClass.m_stage.stageWidth + tmp_width +  Math.floor( Math.random() * tmp_width  ) + 50;
					tmp_direction = false;
				} else {
					tmp_x = -tmp_width*2;
					tmp_direction = true;
				}
				tmp_y = Math.floor( Math.random() * 400 ) + 100;


				var tmp_enemy:Enemy = new Enemy(tmp_x, 
												tmp_y, 
												tmp_height, 
												tmp_width, 
												tmp_direction, 
												5)
				return tmp_enemy;
		}
		
		
		private function chooseEnemy():void {
			if(m_choosen) {
				if(m_currentEnemy != null) {
					if(Enemy(m_currentEnemy).toDestroy()) {
					//if(m_timer > 500) {
						m_timer = 0;
						if(m_currentEnemy != null)
							resetTint(m_currentEnemy);
						m_choosen = false;
					} else {
						m_timer++;
						if(m_currentEnemy != null)
							applyReedTint(m_currentEnemy);
					}
				} else {
					m_choosen = false;
				}
			}
		}
		
		public function update():void {
			for (var i = 0; i < m_enemies.length; i++){
				
				if(!m_choosen) {
					m_currentEnemy = m_enemies[Math.floor( Math.random() * m_enemies.length )];
					m_choosen = true;
				}
				
				MovieClip(m_enemies[i]).update();

				chooseEnemy();
				checkEnemies();
			}
			
		}
		
		private function checkEnemies():void {
			while (m_enemies.length < m_numberOfEnemies){
				var tmp_enemy:Enemy = createEnemy();
				m_enemies.push(tmp_enemy);
				MainClass.m_enemyLayer.addChild(MovieClip(tmp_enemy));
			}
		}
		
	
		private function resetTint(v_enemy:MovieClip):void {
			var m_colorTint:ColorTransform = v_enemy.transform.colorTransform;
			m_colorTint.redOffset = 0;
			m_colorTint.blueOffset = 0;
			m_colorTint.greenOffset = 0;
			v_enemy.transform.colorTransform = m_colorTint;
		}

		private function applyReedTint(v_enemy:MovieClip):void {
			var m_colorTint:ColorTransform = v_enemy.transform.colorTransform;
			m_colorTint.redOffset = 255;
			m_colorTint.blueOffset = 0;
			m_colorTint.greenOffset = 0;
			v_enemy.transform.colorTransform = m_colorTint;
		}
		
	}
	
}
