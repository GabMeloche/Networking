using UnityEngine;

public class Steering : MonoBehaviour
{
    protected Vector3 velocity = Vector3.zero;
    protected float rotation = 0f;

    public Vector3 Velocity { get { return velocity; } }
    public float Rotation { get { return rotation; } }

    public Vector3 TargetPos { get { return movement.TargetPos; } }

    protected Movement movement;

    [SerializeField]
    float maxAcceleration = 1f;

    void Start()
    {
        movement = GetComponent<Movement>();
    }

    public void Compute()
    {
        if (movement.HasArrived)
        {
            velocity = Vector3.zero;
            return;
        }

        velocity = TargetPos - movement.Position;

        if (velocity.sqrMagnitude > maxAcceleration * maxAcceleration)
        {
            velocity.Normalize();
            velocity *= maxAcceleration;
        }
        velocity.y = 0.0f;
    }
}
