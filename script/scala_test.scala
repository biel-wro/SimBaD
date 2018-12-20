import scala.Enumeration
import scala.collection.mutable.ListBuffer

object EventKind extends Enumeration { 
  type EventKind = Value 
  val None, Created, Removed, Modified, Transformed, JumpedOut, JumpedIn = Value 
  
  def fromInt(x : Int) : EventKind.Value = x match {
    case 0 => EventKind.None
    case 1 => EventKind.Created
    case 2 => EventKind.Removed
    case 3 => EventKind.Modified
    case 4 => EventKind.Transformed
    case 5 => EventKind.JumpedOut
    case 6 => EventKind.JumpedIn
  }
  
  def toInt(x : EventKind.Value ) : Int = x match {
    case EventKind.None         => 0
    case EventKind.Created      => 1
    case EventKind.Removed      => 2
    case EventKind.Modified     => 3
    case EventKind.Transformed  => 4
    case EventKind.JumpedOut    => 5
    case EventKind.JumpedIn     => 6
  }
}

case class Position(
  val x: Float, 
  val y: Float, 
  val z: Float
) extends Serializable

case class MutationData(
  val be: Float, 
  val br: Float, 
  val le: Float, 
  val lr: Float, 
  val se: Float, 
  val sr: Float
) extends Serializable

case class Event(
  val time : Double, 
  val deltaTime : Int, 
  val eventKind : Int, 
  val position : Position, 
  val mutation : MutationData
) extends Serializable

case class NumberedEvent(
  val id: Long,
  val event: Event
)

def lineToEvent(line : String) : Event = {
  val fields = line.split(";")
  
  val position = Position( fields(0).toFloat, fields(1).toFloat, fields(2).toFloat )
  val density = fields(3).toFloat
  val nextEventTime = fields(4).toFloat
  val nextEventKind = fields(5).toInt
  val mutation = MutationData( fields(6).toFloat, fields(7).toFloat, fields(8).toFloat, fields(9).toFloat, fields(10).toFloat, fields(11).toFloat )
  val mutationId = fields(12).toInt
  val birthRate = fields(13).toFloat
  val deathRate = fields(14).toFloat
  val successProbability = fields(15).toFloat
  val lifespan = fields(16).toFloat
  val eventTime = fields(17).toDouble
  val eventDeltaTime = fields(18).toInt
  val eventKind = fields(19).toInt
  
  Event( eventTime, eventDeltaTime, eventKind, position, mutation ) 
}

def lineToMaybeEvent(line : String ) : Option[Event] = {
  try{
    Some(lineToEvent(line))
  } catch{
    case e : Exception => None
  }  
}

val lines = sc.textFile("/data/stream100k.csv.gz")

val events = lines.map(lineToMaybeEvent).filter( x => !x.isEmpty ).map(x => x.get )

val numberedEvents = events.zipWithIndex.map(x => NumberedEvent(x._2,x._1) )

val eventLineages = numberedEvents.groupBy( x => x.event.position )

case class Particle(
  val birthTime: Double, 
  val deathTime: Double,
  val position: Position,
  val mutation: MutationData
) extends Serializable

case class TreeParticle(
  val id: Long,
  val parentId: Long,
  val particle: Particle
) extends Serializable


def eventsToParticles( unsortedEvents : Iterable[NumberedEvent] ) : List[TreeParticle] = {

  var particles = new ListBuffer[TreeParticle]()

  var isAlive = false
  var id: Long = -1    
  var parentId: Long = -1
  var birthTime = Double.NegativeInfinity
  var position: Position = null
  var mutation: MutationData = null 

  var firstRun = true

  for( nEvent <- unsortedEvents.toList.sortBy( e => e.event.time ) ){
    val eventKind = nEvent.event.eventKind
    
    if (firstRun) {
      firstRun = false
      if (eventKind != 1) {
        isAlive = true
        parentId = -1
        birthTime = Double.NegativeInfinity
        position = nEvent.event.position
        mutation = nEvent.event.mutation
      }
    }
    
    eventKind match {
      case 1 => {
        isAlive = true
        id = nEvent.id
        parentId = -1
        birthTime = nEvent.event.time
        position = nEvent.event.position
        mutation = nEvent.event.mutation
      }
      case 2 => {
        val particle = Particle(birthTime, nEvent.event.time, position, mutation)
        particles += TreeParticle(id, parentId, particle)
        isAlive = false
        id = -1
        parentId = -1
      }
      
      case 4 => {
        val particle = Particle(birthTime, nEvent.event.time, position, mutation)
        particles += TreeParticle(id, parentId, particle)
        parentId = id
        id = nEvent.id
        birthTime = nEvent.event.time
        position = nEvent.event.position
        mutation = nEvent.event.mutation
      }
    }
  }
  if (isAlive){
    val particle = Particle(birthTime, Double.NegativeInfinity, position, mutation)
    particles += TreeParticle(id, parentId, particle)
  }
    
  particles.toList
}

val particleLineages = eventLineages.mapValues(eventsToParticles)


val some = particleLineages.take(100)
