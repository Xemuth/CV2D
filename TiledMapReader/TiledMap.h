#ifndef _TiledMapReader_TiledMap_h_
#define _TiledMapReader_TiledMap_h_
#include <Core/core.h>


namespace Upp{

enum class TiledOrientation{Orthogonal, Isometrique, IsometriqueEchelonne, HexagonalEchelonne};
enum class TiledRenderOrder{RightDown, RightTop, LeftDown, LeftUp};
enum class TiledType{Map};
enum class TiledLayerType{TileLayer};

class TiledTilesSet{
	public:
		TiledTilesSet(const Value& jsonTilesSetObject);
		~TiledTilesSet();
		
		int GetFirstGID()const;
		const Upp::String GetSource()const;
	private:
		int firstGid;
		Upp::String source;
};

class TiledLayer{
	public:
		TiledLayer(const Value& jsonLayerObject);
		~TiledLayer();
		
		const Upp::String& GetName()const;
		const Upp::Vector<int>&  GetDatas()const;
		int GetId()const;
		int GetHeight()const;
		int GetWidth()const;
		int GetX()const;
		int GetY()const;
		
		float GetOpacity()const;
		bool IsVisible()const;
		
		TiledLayerType GetType()const;
		const ValueMap& GetProperties()const;
		
	private:
		Upp::String name;
		Upp::Vector<int> datas;
		bool visible;
		float opacity;
		
		int height;
		int width;
		int id;
		int x;
		int y;
		
		TiledLayerType type;
		ValueMap properties;
};

class TiledMapJson{
	public:
		TiledMapJson(const Upp::String& file);
		~TiledMapJson();
		
		const Upp::String& GetVersion()const;
		const Upp::String& GetTiledVersion()const;
		
		const Upp::Array<TiledLayer>& GetLayers()const;
		const Upp::Array<TiledTilesSet>& GetTilesSets()const;
		
		TiledType GetTiledType()const;
		TiledOrientation GetTiledOrientation()const;
		TiledRenderOrder GetRenderOrder()const;
		
		bool IsInfinite()const;
		int GetCompressionLevel()const;
		int GetTileHeight()const;
		int GetTileWidgth()const;
		int GetWidth()const;
		int GetHeight()const;
		int GetNextLayerId()const;
		int GetNextObjectId()const;
	private:
		Upp::String version;
		Upp::String tiledVersion;
		
		Upp::Array<TiledLayer> layers;
		Upp::Array<TiledTilesSet> tilesSets;
		
		int compressionLevel;
		bool infinite;
		
		TiledType type;
		TiledOrientation orientation;
		TiledRenderOrder renderOrder;

		int tileHeight;
		int tileWidth;
		
		int width;
		int height;
		
		int nextLayerid;
		int nextObjectid;
};

}

#endif
