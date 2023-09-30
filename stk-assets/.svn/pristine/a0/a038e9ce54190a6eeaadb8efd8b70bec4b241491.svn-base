/*
void onLibraryNodeCollision(int idKart, const string libraryInstance, const string objID)
{
    Utils::logInfo("Collision! Kart " + idKart + " with obj " + objID + " from " + libraryInstance);
}
*/

void enableYellowInGreen(bool enable)
{
    Track::TrackObject@ to = Track::getTrackObject("", "MovableYellowInGreen");
    to.setEnabled(enable);
}

void enableListInst3(bool enable)
{
    Track::TrackObject@ to = Track::getTrackObject("LibInst3", "Cube.001");
    to.setEnabled(enable);
}


void testBlowUpWall()
{
    blowUpWall("Wall_proxy");
}

void testMove()
{
    Track::TrackObject@ obj = Track::getTrackObject("", "LibInst3");
    Vec3 coord(0,0,0);
    obj.moveTo(coord, true);
}

void testParticlesRate()
{
    Track::TrackObject@ obj = Track::getTrackObject("", "TestEmitter");
    Track::ParticleEmitter@ emitter = obj.getParticleEmitter();
    emitter.setEmissionRate(1.0);
    emitter.stopIn(3.0);
}


void onCylinderTrigger(int idKart)
{
    Utils::logInfo("Collision with cylinder");
}

void setLightEnergy(float energy)
{
    Track::TrackObject@ obj = Track::getTrackObject("", "Lamp");
    Track::Light@ light = obj.getLight();
    light.setEnergy(energy);
}

void animateLightEnergy(float energy)
{
    Track::TrackObject@ obj = Track::getTrackObject("", "Lamp");
    Track::Light@ light = obj.getLight();
    light.animateEnergy(energy, 5.0);
}

void setThickFog(bool thick)
{
    if (thick)
        Track::setFog(1.0, 5.0, 50.0, 95, 117, 176, 5.0);
    else
        Track::setFog(0.5, 50.0, 300.0, 95, 117, 176, 5.0);
}

void setRedFog()
{
    Track::setFog(1.0, 5.0, 50.0, 255, 0, 0, 5.0);
}