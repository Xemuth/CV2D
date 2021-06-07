#include "TiledMap.h"

namespace Upp{

TiledMapJson::TiledOrientation ConverStringToTiledOrientation(const Upp::String& orientation);
TiledMapJson::TiledRenderOrder ConverStringToTiledRenderOrder(const Upp::String& renderOrder);
TiledMapJson::TiledType ConverStringToTiledType(const Upp::String& type);
TiledLayer::TiledLayerType ConverStringToTiledLayerType(const Upp::String& layerType);

TiledMapJson::TiledMapJson(const Upp::String& file) noexcept(false){
	ASSERT_(FileExists(file), "JSON File \"" + file + "\" don't exist");
	try{
		path = file;
		data = LoadFile(file);
		Value json = ParseJSON(data);
		
		version = json["version"];
		tiledVersion = json["tiledversion"];
		
		compressionLevel = json["compressionlevel"];
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
		
		ReadLayers(json["layers"]);
		ReadTilesSets(json["tilesets"]);
	}catch(Upp::Exc& exception){
		throw exception;
	}
}

TiledMapJson::TiledMapJson(TiledMapJson&& map) : data(pick(map.data)), path(pick(map.path)){
	version = map.version;
	tiledVersion = map.tiledVersion;
	
	compressionLevel = map.compressionLevel;
	infinite = map.infinite;
	
	type = map.type;
	orientation = map.orientation;
	renderOrder = map.renderOrder;

	tileHeight = map.tileHeight;
	tileWidth = map.tileWidth;
	
	width = map.width;
	height = map.height;

	nextLayerid = map.nextLayerid;
	nextObjectid = map.nextObjectid;
	
	for(TiledLayer& layer : map.layers)
		layers.Create(pick(layer));
	
	for(TiledTilesSet& tileSet : map.tilesSets)
		tilesSets.Create(pick(tileSet));
}



void TiledMapJson::ReadLayers(const Value& jsonLayers){
	for(int i = 0; i < jsonLayers.GetCount(); i++){
		layers.Create(jsonLayers[i]);
	}
}

void TiledMapJson::ReadTilesSets(const Value& jsonTilesSets){
	for(int i = 0; i < jsonTilesSets.GetCount(); i++){
		tilesSets.Create(jsonTilesSets[i]);
	}
}

TiledLayer::TiledLayer(const Value& jsonLayerObject){
	name = jsonLayerObject["name"];
	for(int i = 0; i < jsonLayerObject["data"].GetCount(); i++)
		datas.Add(jsonLayerObject["data"][i]);
		
	visible = jsonLayerObject["visible"];
	opacity = jsonLayerObject["opacity"].Get<double>();
	
	height = jsonLayerObject["height"];
	width = jsonLayerObject["width"];
	id = jsonLayerObject["id"];
	x = jsonLayerObject["x"];
	y = jsonLayerObject["y"];
	
	type = ConverStringToTiledLayerType(jsonLayerObject["type"]);
	
	if(!IsNull(jsonLayerObject["properties"]))
		properties = jsonLayerObject["properties"];
}

TiledLayer::TiledLayer(TiledLayer&& layer){
	name = layer.name;
	for(int i : layer.datas)
		datas.Add(i);
	
	visible = layer.visible;
	opacity = layer.opacity;
	
	height = layer.height;
	width = layer.width;
	id = layer.id;
	x = layer.x;
	y = layer.y;
	
	type = layer.type;
	properties = layer.properties;
}

TiledTilesSet::TiledTilesSet(const Value& jsonTilesSetObject){
	firstGid = jsonTilesSetObject["firstGid"];
	source = jsonTilesSetObject["source"];
}

TiledTilesSet::TiledTilesSet(TiledTilesSet&& set){
	firstGid = set.firstGid;
	source = set.source;
}

TiledMapJson::TiledOrientation ConverStringToTiledOrientation(const Upp::String& orientation){
	if(ToLower(orientation).IsEqual("staggered"))
		return TiledMapJson::TiledOrientation::Staggered;
	else if(ToLower(orientation).IsEqual("hexagonal"))
		return TiledMapJson::TiledOrientation::Hexagonal;
	else if(ToLower(orientation).IsEqual("isometrique"))
		return TiledMapJson::TiledOrientation::Isometrique;
	else
		return TiledMapJson::TiledOrientation::Orthogonal;
}

TiledMapJson::TiledRenderOrder ConverStringToTiledRenderOrder(const Upp::String& renderOrder){
	if(ToLower(renderOrder).IsEqual("left-down"))
		return TiledMapJson::TiledRenderOrder::LeftDown;
	else if(ToLower(renderOrder).IsEqual("left-up"))
		return TiledMapJson::TiledRenderOrder::LeftUp;
	else if(ToLower(renderOrder).IsEqual("right-up"))
		return TiledMapJson::TiledRenderOrder::RightUp;
	else
		return TiledMapJson::TiledRenderOrder::RightDown;
}

TiledMapJson::TiledType ConverStringToTiledType(const Upp::String& type){
	if(ToLower(type).IsEqual("map"))
		return TiledMapJson::TiledType::Map;
	else
		return TiledMapJson::TiledType::Map;
}

TiledLayer::TiledLayerType ConverStringToTiledLayerType(const Upp::String& layerType){
	if(ToLower(layerType).IsEqual("tilelayer"))
		return TiledLayer::TiledLayerType::TileLayer;
	else
		return TiledLayer::TiledLayerType::TileLayer;
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
TiledLayer::TiledLayerType TiledLayer::GetType()const{return type;}
const ValueMap& TiledLayer::GetProperties()const{return properties;}
TiledMapJson::~TiledMapJson(){}
const Upp::String& TiledMapJson::GetPath()const{return path;}
const Upp::String& TiledMapJson::GetData()const{return data;}
const Upp::String& TiledMapJson::GetVersion()const{return version;}
const Upp::String& TiledMapJson::GetTiledVersion()const{return tiledVersion;}
const Upp::Array<TiledLayer>& TiledMapJson::GetLayers()const{return layers;}
const Upp::Array<TiledTilesSet>& TiledMapJson::GetTilesSets()const{return tilesSets;}
TiledMapJson::TiledType TiledMapJson::GetTiledType()const{return type;}
TiledMapJson::TiledOrientation TiledMapJson::GetTiledOrientation()const{return orientation;}
TiledMapJson::TiledRenderOrder TiledMapJson::GetRenderOrder()const{return renderOrder;}
bool TiledMapJson::IsInfinite()const{return infinite;}
int TiledMapJson::GetCompressionLevel()const{return compressionLevel;}
int TiledMapJson::GetTileHeight()const{return tileHeight;}
int TiledMapJson::GetTileWidth()const{return tileWidth;}
int TiledMapJson::GetWidth()const{return width;}
int TiledMapJson::GetHeight()const{return height;}
int TiledMapJson::GetNextLayerId()const{return nextLayerid;}
int TiledMapJson::GetNextObjectId()const{return nextObjectid;}

}