import org.apache.spark.sql.Row
import org.apache.spark.sql.types._

//val pathPrefix = "/scratch/dragons_food/neutral_evolution/10M-birth-efficiency-only-mutation-0.002/"
//val numTimePoints = 100;

val chronicleSchema = StructType(Array(
  StructField("id", LongType, false),
  StructField("parent_id", LongType, false),
  StructField("birth_time", DoubleType, false),
  StructField("death_time", DoubleType, false),
  StructField("position_0", FloatType, false),
  StructField("position_1", FloatType, false),
  StructField("position_2", FloatType, false),
  StructField("mutation_id", LongType, false),
  StructField("birth_efficiency", FloatType, false),
  StructField("birth_resistance", FloatType, false),
  StructField("lifespan_efficiency", FloatType, false),
  StructField("lifespan_resistance", FloatType, false),
  StructField("success_efficiency", FloatType, false),
  StructField("success_resistance", FloatType, false)
));

val chronicles = spark.read.
  format("csv").
  option("positiveInf", "inf").
  option("negativeInf", "-inf").
  option("header","true").
  option("delimiter",";").
  option("mode","DROPMALFORMED").
  schema(chronicleSchema).
  load(pathPrefix + "/chronicles.csv.gz").
  cache;
  
val maxTime = chronicles.agg( max("birth_time") ).collect()(0).getDouble(0);

val uglyTree =  chronicles.
  select("id", "parent_id", "mutation_id").alias("children").
    join(chronicles.select("id","mutation_id").alias("parents"), $"children.parent_id"===$"parents.id", "left" );         

val tree = ( ugly_tree.map( r => (
  r.getLong(0),
  r.getLong(2),
  if (r.isNullAt(3)) 0 else r.getLong(3),
  if (r.isNullAt(4)) 0 else r.getLong(4)
))
  .withColumnRenamed("_1", "id")
  .withColumnRenamed("_2", "mutation")
  .withColumnRenamed("_3","parent")
  .withColumnRenamed("_4","parent_mutation")
)

val mutation_tree = (
  tree.filter( $"mutation" =!= $"parent_mutation")
  .dropDuplicates("mutation")
  .withColumnRenamed("id","first_particle_id")
  .withColumnRenamed("parent_id","first_particle_parent")
)


mutation_tree.
  sort("mutation").
  select("mutation", "parent_mutation").
  coalesce(1).
  write.
  text(pathPrefix + "/tree.txt");


// snapshots
for( t <- 0 to numTimePoints ){
  chronicles.
  filter( (col("birth_time") < t) && (col("death_time") > t)  ).
  coalesce(1).
  write.
  format("csv").
  option("delimiter",";").
  option("header", "true").
  mode("overwrite").
  save(pathPrefix + "/snapshots/" + t.toString() )  
  
}
  
  
// final
chronicles.filter( col("death_time") === Double.PositiveInfinity ).
  select(
    $"position_0", $"position_1", $"position_2", 
    $"mutation_id",
    $"birth_efficiency", $"birth_resistance", 
    $"lifespan_efficiency", $"lifespan_resistance", 
    $"success_efficiency", $"success_resistance").
  coalesce(1).
  write.
  format("csv").
  option("delimiter",";").
  option("header","true").
  mode("overwrite").
  save(pathPrefix + "/final")  
  
  
val snapshotsUdf = udf( (t1:Double, t2:Double) => (0d to numTimePoints by 1).filter( t => t1 < t && t < t2 ) )

val snapshots = chronicles.withColumn("time_point", explode(snapshotsUdf($"birth_time", $"death_time") ))

val time_stats = snapshots.groupBy("time_point").agg(
  count(lit(1)).alias("count"), 
  //max( sqrt( $"position_0"*$"position_0" + $"position_1"*$"position_1" + $"position_2"*$"position_2")),
  // means
  avg($"birth_efficiency").alias("mean_birth_efficiency"), 
  avg($"birth_resistance").alias("mean_birth_resistance"), 
  avg($"lifespan_efficiency").alias("mean_lifespan_efficiency"), 
  avg($"lifespan_resistance").alias("mean_lifespan_resistance"), 
  avg($"success_efficiency").alias("mean_sucdess_efficiency"), 
  avg($"success_resistance").alias("mean_success_resistance"),
  
  // stdev
  stddev($"birth_efficiency").alias("stddev_birth_efficiency"), 
  stddev($"birth_resistance").alias("stddev_birth_resistance"), 
  stddev($"lifespan_efficiency").alias("stddev_lifespan_efficiency"), 
  stddev($"lifespan_resistance").alias("stddev_lifespan_resistance"), 
  stddev($"success_efficiency").alias("stddev_sucdess_efficiency"), 
  stddev($"success_resistance").alias("stddev_success_resistance")
).orderBy("time_point")

time_stats.coalesce(1).
  write.
  format("csv").
  option("delimiter",";").
  option("header","true").
  mode("overwrite").
  save(pathPrefix + "/time_stats");



