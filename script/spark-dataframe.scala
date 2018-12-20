val df =  (spark.read
                .format("csv")
                .option("header","true")
                .option("delimiter",";")
                .option("inferSchema","true")
                .option("mode","DROPMALFORMED")
                .load("/data/stream100k.csv")
              );
class Position( val x :Double, val y :Double, val z :Double ){
  def canEqual(a:Any) = a.isInstanceOf[Position]
  override def toString : String = { "[" + x + "," + y + "," + z +"]" }
  override def equals( that : Any) : Boolean = { 
    if (!isInstanceOf[Position])
      return false 
    val other = that.asInstanceOf[Position]
    return x == other.x  && y == other.y && z == other.z
  }
    
  override def hashCode : Int = { x.hashCode + 31 * y.hashCode + 113 * z.hashCode }
}

def loadEventStream(stream_path : String, delimiter : String = ";") = {
  val initial =  spark.read.format("csv")
              .option("header","true")
              .option("delimiter", delimiter)
              .option("mode","DROPMALFORMED")
              .load(stream_path);

  val position_from_strings = udf( x:String,y:String, z:String => Array(x.toDouble,y.toDouble,z.toDouble) )            

  //val parsed = initial.withColumn("position"
                  
                  
}

val myUDF = udf( (x:Double,y:Double,z:Double) => Array(x,y,z) )
                
                
def dfZipWithIndex(
  df: DataFrame,
  offset: Int = 1,
  colName: String = "id",
  inFront: Boolean = true
) : DataFrame = {
  df.sqlContext.createDataFrame(
    df.rdd.zipWithIndex.map(ln =>
      Row.fromSeq(
        (if (inFront) Seq(ln._2 + offset) else Seq())
          ++ ln._1.toSeq ++
        (if (inFront) Seq() else Seq(ln._2 + offset))
      )
    ),
    StructType(
      (if (inFront) Array(StructField(colName,LongType,false)) else Array[StructField]()) 
        ++ df.schema.fields ++ 
      (if (inFront) Array[StructField]() else Array(StructField(colName,LongType,false)))
    )
  ) 
}