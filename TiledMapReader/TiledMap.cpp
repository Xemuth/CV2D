#include "TiledMap.h"

namespace Upp{

TiledMapJson::TiledMapJson(const Upp::String& file){
		
		if(!IsFullPath(file)) ASSERT_(FileExists("./" + file), "File " + file + " don't exist");
		else ASSERT_(FileExists(file), "File " + file + " don't exist");
		
		Value json = ParseJSON(LoadFile(file));
		
		version = json["version"];
		tiledVersion = json["tiledVersion"];
		
		compressionLevel = json["compressionLevel"];
		infinite = json["infinite"];
		
		type = ConverStringToTiledType(json["type"]);
		orientation = ConverStringToTiledOrientation(json["orientation"]);
		renderOrder = ConverStringToTiledRenderOrder(json["renderorder"]);
	
		tileHeight = json["tileheight"];
		tileWidth = json["tilewidth"];
		
		width = json["width"];
		height = json["height"];
		
		nextLayerid = json["nextlayerid"];
		nextObjectid = json["nextobjectid"];
	
		//Upp::Array<TiledLayer> layers;
		//Upp::Array<TiledTilesSet> tilesSets;
}

TiledLayer::TiledLayer(const Value& jsonLayerObject){
	
}

TiledTilesSet::TiledTilesSet(const Value& jsonTilesSetObject){
		
}

TiledOrientation ConverStringToTiledOrientation(const Upp::String& orientation){
	if(ToLower(orientation).IsEqual("staggered"))
		return TiledOrientation::Staggered;
	else if(ToLower(orientation).IsEqual("hexagonal"))
		return TiledOrientation::Hexagonal;
	else if(ToLower(orientation).IsEqual("isometrique"))
		return TiledOrientation::Isometrique;
	else
		return TiledOrientation::Orthogonal;
}

TiledRenderOrder ConverStringToTiledRenderOrder(const Upp::String& renderOrder){
	if(ToLower(renderOrder).IsEqual("left-down"))
		return TiledRenderOrder::LeftDown;
	else if(ToLower(renderOrder).IsEqual("left-up"))
		return TiledRenderOrder::LeftUp;
	else if(ToLower(renderOrder).IsEqual("right-up"))
		return TiledRenderOrder::RightUp;
	else
		return TiledRenderOrder::RightDown;
}

TiledType ConverStringToTiledType(const Upp::String& type){
	if(ToLower(type).IsEqual("map"))
		return TiledType::Map;
	else
		return TiledType::Map;
}

TiledLayerType ConverStringToTiledLayerType(const Upp::String& layerType){
	if(ToLower(layerType).IsEqual("tilelayer"))
		return TiledLayerType::TileLayer;
	else
		return TiledLayerType::TileLayer;
}

TiledTilesSet::~TiledTilesSet(){}
int TiledTilesSet::GetFirstGID()const{return firstGid;}
const Upp::String TiledTilesSet::GetSource()const{return source;}
TiledLayer::~TiledLayer(){}
const Upp::String& TiledLayer::GetName()const{return name;}
const Upp::Vector<int>&  TiledLayer::GetDatas()const{return datas;}
int TiledLayer::GetId()const{return id;}
int TiledLayer::GetHeight()const{return height;}
int TiledLayer::GetWidth()const{return width;}
int TiledLayer::GetX()const{return x;}
int TiledLayer::GetY()const{return y;}
float TiledLayer::GetOpacity()const{return opacity;}
bool TiledLayer::IsVisible()const{return visible;}
TiledLayerType TiledLayer::GetType()const{return type;}
const ValueMap& TiledLayer::GetProperties()const{return properties;}
TiledMapJson::~TiledMapJson(){}
const Upp::String& TiledMapJson::GetVersion()const{return version;}
const Upp::String& TiledMapJson::GetTiledVersion()const{return tiledVersion;}
const Upp::Array<TiledLayer>& TiledMapJson::GetLayers()const{return layers;}
const Upp::Array<TiledTilesSet>& TiledMapJson::GetTilesSets()const{return tilesSets;}
TiledType TiledMapJson::GetTiledType()const{return type;}
TiledOrientation TiledMapJson::GetTiledOrientation()const{return orientation;}
TiledRenderOrder TiledMapJson::GetRenderOrder()const{return renderOrder;}
bool TiledMapJson::IsInfinite()const{return infinite;}
int TiledMapJson::GetCompressionLevel()const{return compressionLevel;}
int TiledMapJson::GetTileHeight()const{return tileHeight;}
int TiledMapJson::GetTileWidgth()const{return tileWidth;}
int TiledMapJson::GetWidth()const{return width;}
int TiledMapJson::GetHeight()const{return height;}
int TiledMapJson::GetNextLayerId()const{return nextLayerid;}
int TiledMapJson::GetNextObjectId()const{return nextObjectid;}

}