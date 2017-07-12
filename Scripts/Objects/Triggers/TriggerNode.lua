--[[
	Dato che la funzione collision è l'unica che può inserire l'entità all'interno dello script
	dovrò usare il SetUserdata per poter richiamare i metodi delle mie classi
	così senza specificare che si tratti dell'attore questo script potrà avere effetti su tutte le entity
	idonee( umani in particolare )
	
	ATTENZIONE GetUserdata non è esposta ovviamente, devo farne una io che casta con il tipo che mi aspetto
]]
-- un giorno sarà:
-- local Actor = MyCastFunc( Entity:GetUserdata(), "Actor" );
-- Actor():AsLiveEntity():ClearStates();

-- Adesso è:
-- is cEntity
-- self.pEntity = Utils.GetAsEntity( entity );
-- self.pEntity:As"LiveEntity":SetClimbing();

-- https://www.youtube.com/watch?v=gZ36ugnRex4

Script.pLinkNode 	= nil; 			--Entity "Linked Node"
Script.fInRadius	= 0.7; 			--Float "Enter Radius"
Script.fOutRadius	= 1.0; 			--Float "Exit Radius"

Script.bLinked		= false;
Script.pEntity		= nil;
Script.IsColliding 	= false;
Script.bMustLeave 	= false;
Script.bHasExited 	= false;
Script.bDestination = false;

function Script:GetName() return self.entity:GetKeyValue( "name" ); end

function Script:Reset()
	self.bLinked		= false;
	self.pEntity		= nil;
	self.IsColliding 	= false;
	self.bMustLeave 	= false;
	self.bHasExited 	= false;
	self.bDestination	= false;
end

function Script:Exit()
	
	if ( self.pEntity ) then
		self.pEntity:As"LiveEntity":SetPlainWalking();
	end
	
	self.pLinkNode.script:Reset();
	self:Reset();
end

function Script:FixedUpdate()
	
	-- no more collision, so entity has leaved collision area, now can re use this trigger
	if ( not self.IsColliding and self.bMustLeave )  then
		self.bMustLeave = false;
	end
	
	if ( not self.bLinked ) then self.IsColliding = false; return; end
	
	-- if user coming back exit
	if ( InputManager.Backward() and not self.bMustLeave and self.IsColliding ) then
		self:Exit();
	end
	
	self.IsColliding = false;
end

function Script:Collision( entity, Position, Normal, Speed )
	
	if ( self.IsColliding ) then return; end
	
	self.IsColliding = true;
	
	-- user must leave collision area in order to re use trigger
---	if ( self.bMustLeave ) then return; end

--	if ( self.bLinked ) then
--		print( self:GetName() .. " " .. ( self.bDestination and "true" or "false" ) );
--	end
	--[[
	if ( self.bLinked and self.bDestination  ) then -- coming from a node, exiting
		print( "Has to go out" );
		-- garant more precision when exiting from trigger
		if ( entity:GetDistance( self.entity ) > ( self.fOutRadius ) ) then return; end
		
		-- remove every force and stop entity
		self.pEntity:Stop();
		self:Exit();
		return;
		
	end
	]]
	if ( not self.bLinked ) then -- user want use trigger
		
		-- garant precision when entering trigger
		if ( entity:GetDistance( self.entity ) > self.fInRadius ) then return; end
		if ( not InputManager.Use() ) then return; end
		
		-- is cEntity
		self.pEntity = Utils.GetAsEntity( entity );
		
		-- remove every force and stop entity
		self.pEntity:Stop();
		
		-- set the new direction
		local vRotation = self.pLinkNode:GetPosition() - self.entity:GetPosition();
		self.pEntity:SetRotationP1ToP2( vRotation ); -- As cEntity
		
		-- set entity position to trigger node position
		entity:SetPosition( self.entity:GetPosition() );
		
		-- Set entity state as Climbing
		self.pEntity:As"LiveEntity":SetClimbing(); -- good alias for point to point movement
		
		-- link the entity to the next point
		self.pLinkNode.script.pEntity = self.pEntity;
		
		-- set flag for linked condition
		self.pLinkNode.script.bLinked = true;
		
		-- Link to this point
		self.bLinked = true;
	end
	
end