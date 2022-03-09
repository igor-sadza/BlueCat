//TODO
//  - 3 progi odpowiadaja za pokazanie kursora, czy mamy śledzić, czy możemy zabić
//  - dodac podstawowe ui, jakies menu?
//  - dodac muzyke
//  - dodać ekran powitalny, puste obiekty, font dodasz bezposrendio 
//  - walidacja z sql
// 

package  {
	import flash.display.*;
	import flash.events.*;
	import flash.display.*;
	
	public class MainClass extends MovieClip {
		
		//////////////////////////////////////
		// VARIABLES
		//////////////////////////////////////
		
		public static var m_stage:Stage;
		private var m_enemyManager:EnemyManager;
		private var m_player:Player;
		private var m_menu:uiMenu;
		//private var m_background:Background;
		
		public static var m_backgroundLayer:Sprite;
		public static var m_enemyLayer:Sprite;
		public static var m_playerLayer:Sprite;
		public static var m_uiLayer:Sprite;
		
		var m_background_0_0:Background
		var m_background_0_1:Background
		
		var m_background_1_0:Background
		var m_background_1_1:Background
		
		var m_background_2_0:Background
		var m_background_2_1:Background
		
		var m_background_3_0:Background
		
		var m_background_4_0:Background
		var m_background_4_1:Background
		
		var m_background_5_0:Background
		var m_background_5_1:Background
		
		var m_background_6_0:Background
		var m_background_6_1:Background
		
		var m_background_7_0:Background
		var m_background_7_1:Background
		
		//////////////////////////////////////
		// CONSTRUTOR
		//////////////////////////////////////
		public function MainClass():void {
			stage.addEventListener(Event.ENTER_FRAME, enterFrameHandler);
			stage.addEventListener(MouseEvent.MOUSE_DOWN, onMouseDownHandler);
			
			m_backgroundLayer		= new Sprite();
			m_enemyLayer			= new Sprite();
			m_playerLayer			= new Sprite();
			m_uiLayer				= new Sprite();
			
			m_stage 				= stage;
			
			addChildAt	( m_backgroundLayer,0);
			addChildAt	(m_enemyLayer,1);
			addChildAt	(m_playerLayer,2);
			addChildAt	(m_uiLayer,3);
			
			
			m_enemyManager = new EnemyManager(m_enemyLayer, 4);
			m_player = new Player(m_playerLayer, m_enemyManager);
			m_menu = new uiMenu(m_uiLayer);
			m_uiLayer.addChild(m_menu);
			
			
			
			//BACKGROUND
			
			m_background_3_0 = new Background(4);
			m_backgroundLayer.addChild(m_background_3_0);
			
			m_background_7_0 = new Background(7);
			m_background_7_1 = new Background(7);
			
			m_background_7_0 .x -= m_background_7_0 .width / 2;
			m_background_7_0 .y = 0;
			
			m_background_7_1.x = m_background_7_1.width / 2;
			m_background_7_1.y = 0;
			
			m_backgroundLayer.addChild(m_background_7_0);
			m_backgroundLayer.addChild(m_background_7_1);
			
			m_background_6_0 = new Background(6);
			m_background_6_1 = new Background(6);
			
			m_background_6_0 .x -= m_background_6_0 .width / 2;
			m_background_6_0 .y = 0;
			
			m_background_6_1.x = m_background_6_1.width / 2;
			m_background_6_1.y = 0;
			
			m_backgroundLayer.addChild(m_background_6_0);
			m_backgroundLayer.addChild(m_background_6_1);
			
			m_background_5_0 = new Background(5);
			m_background_5_1 = new Background(5);
			
			m_background_5_0 .x -= m_background_5_0 .width / 2;
			m_background_5_0 .y = 0;
			
			m_background_5_1.x = m_background_5_1.width / 2;
			m_background_5_1.y = 0;
			
			m_backgroundLayer.addChild(m_background_5_0);
			m_backgroundLayer.addChild(m_background_5_1);
			
			

			
			m_background_0_0 = new Background(3);
			m_background_0_1 = new Background(3);
			
			m_background_0_0.x -= m_background_0_0.width / 2;
			m_background_0_0.y = 0;
			
			m_background_0_1.x = m_background_0_1.width / 2;
			m_background_0_1.y = 0;
			
			
			m_backgroundLayer.addChild(m_background_0_0);
			m_backgroundLayer.addChild(m_background_0_1);
		
		
			m_background_1_0 = new Background(2);
			m_background_1_1 = new Background(2);
			
			m_background_1_0.x -= m_background_1_0.width / 2;
			m_background_1_0.y = 0;
			
			m_background_1_1.x = m_background_1_1.width / 2;
			m_background_1_1.y = 0;
			
			
			m_backgroundLayer.addChild(m_background_1_0);
			m_backgroundLayer.addChild(m_background_1_1);
			
			
			m_background_2_0 = new Background(1);
			m_background_2_1 = new Background(1);
			
			m_background_2_0.x -= m_background_2_0.width / 2;
			m_background_2_0.y = 0;
			
			m_background_2_1.x = m_background_2_1.width / 2;
			m_background_2_1.y = 0;
			
			
			m_backgroundLayer.addChild(m_background_2_0);
			m_backgroundLayer.addChild(m_background_2_1);		
			
			m_background_4_0 = new Background(5);
			m_background_4_1 = new Background(5);
			
			m_background_4_0 .x -= m_background_4_0 .width / 2;
			m_background_4_0 .y = 0;
			
			m_background_4_1.x = m_background_4_1.width / 2;
			m_background_4_1.y = 0;
			
			m_backgroundLayer.addChild(m_background_4_0);
			m_backgroundLayer.addChild(m_background_4_1);
		}
		
		//////////////////////////////////////
		// DOCUMENT METHODS
		//////////////////////////////////////
		
		
		
		//////////////////////////////////////
		// EVENT HANDLERS
		//////////////////////////////////////	
		
		//
		//	Enter Frame handler
		//	@param	event	Uses Event
		//
		private function enterFrameHandler(event:Event):void {
			m_enemyManager.update();
			m_player.setPlayerTarget(m_enemyManager.m_currentEnemy);		
			m_player.update();						
		
		
			m_background_0_0.x -= 4.0;
			m_background_0_1.x -= 4.0;
			if(m_background_0_0.x == -m_background_0_0.width) {
				m_background_0_0.x = this.stage.stageWidth;
			}
			
			if(m_background_0_1.x == -m_background_0_1.width) {
				m_background_0_1.x = this.stage.stageWidth;
			}
			
			
			m_background_1_0.x -= 3.5;
			m_background_1_1.x -= 3.5;
			if(m_background_1_0.x == -m_background_1_0.width) {
				m_background_1_0.x = this.stage.stageWidth;
			}
			
			if(m_background_1_1.x == -m_background_1_1.width) {
				m_background_1_1.x = this.stage.stageWidth;
			}
			
			
			m_background_2_0.x -= 3;
			m_background_2_1.x -= 3;
			if(m_background_2_0.x == -m_background_2_0.width) {
				m_background_2_0.x = this.stage.stageWidth;
			}
			
			if(m_background_2_1.x == -m_background_2_1.width) {
				m_background_2_1.x = this.stage.stageWidth;
			}
			
			m_background_4_0.x -= 2.5;
			m_background_4_1.x -= 2.5;
			if(m_background_4_0.x == -m_background_4_0.width) {
				m_background_4_0.x = this.stage.stageWidth;
			}
			
			if(m_background_4_1.x == -m_background_4_1.width) {
				m_background_4_1.x = this.stage.stageWidth;
			}
			
			m_background_5_0.x -= 2;
			m_background_5_1.x -= 2;
			if(m_background_5_0.x == -m_background_5_0.width) {
				m_background_5_0.x = this.stage.stageWidth;
			}
			
			if(m_background_5_1.x == -m_background_5_1.width) {
				m_background_5_1.x = this.stage.stageWidth;
			}
			
			m_background_6_0.x -= 1.5;
			m_background_6_1.x -= 1.5;
			if(m_background_6_0.x == -m_background_6_0.width) {
				m_background_6_0.x = this.stage.stageWidth;
			}
			
			if(m_background_6_1.x == -m_background_6_1.width) {
				m_background_6_1.x = this.stage.stageWidth;
			}
			
			m_background_7_0.x -= 1;
			m_background_7_1.x -= 1;
			if(m_background_7_0.x == -m_background_7_0.width) {
				m_background_7_0.x = this.stage.stageWidth;
			}
			
			if(m_background_7_1.x == -m_background_7_1.width) {
				m_background_7_1.x = this.stage.stageWidth;
			}
			
		}
		
		private function onMouseDownHandler(event:MouseEvent):void 
		{
			// create new enemy at mouse X & Y
			//_enemyManager.createEnemy(event.stageX, event.stageY, "New Enemy");
			//m_player.setPlayerPosition(event.stageX, event.stageY);
			m_player.spawnPlayerNearTarget();
			
		}
	}
}
