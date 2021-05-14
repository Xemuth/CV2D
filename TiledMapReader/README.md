# TiledMapReader

Simple Tiled map file format reader for C++. It use U++ as base library.

```C++
class TiledMapJson{
	public:
		TiledMapJson(const Upp::String& file);
		
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
    ...
```


