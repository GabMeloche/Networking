using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class Movement : MonoBehaviour
{
    [SerializeField]
    float SlowingDistance = 2f;
    [SerializeField]
    float MinDistToStop = 0.5f;
    [SerializeField]
    float MaxSpeed = 10f;
    [SerializeField]
    float posOffsetY = 0.5f;
    [SerializeField]
    float boostDuration = 2f;
    [SerializeField]
    float boostDecreaseRate = 20f;

    Coroutine boostCoroutine = null;
    float crtMaxSpeed = 10f;
    Rigidbody rb;
    Steering steering;
    Vector3 steeringVelocity = Vector3.zero;
    Vector3 velocity = Vector3.zero;
    float rotation = 0f;
    bool hasArrived = false;
    public bool HasArrived
    {
        get { return hasArrived; }
        private set { hasArrived = rb.isKinematic = value; }
    }

    Vector3 targetPos;
    public Vector3 TargetPos
    {
        get { return targetPos; }
        set
        {
            targetPos = value;
            targetPos.y += posOffsetY;
        }
    }

    public Vector3 Position { get { return transform.position; } }

    void Awake()
    {
        steering = GetComponent<Steering>();
        rb = GetComponent<Rigidbody>();
    }

    void Start()
    {
        targetPos = transform.position;
        // keep position above the floor
        RaycastHit hitInfo = new RaycastHit();
        if (Physics.Raycast(transform.position + Vector3.up * 10, Vector3.down, out hitInfo, 100, 1 << LayerMask.NameToLayer("Floor")))
        {
            transform.position = hitInfo.point + Vector3.up * posOffsetY;
        }
        crtMaxSpeed = MaxSpeed;
    }

    private void OnDestroy()
    {
        if (boostCoroutine != null)
            StopCoroutine(boostCoroutine);
    }

    bool IsArriving()
    {
        if ((TargetPos - Position).magnitude <= SlowingDistance)
            return true;
        return false;
    }

    public void Stop()
	{
		velocity = Vector3.zero;
	}

    float GetSqrMaxSpeed()
    {
        return crtMaxSpeed * crtMaxSpeed;
    }

    float GetOrientationFromDirection(Vector3 direction)
    {
        if (direction.magnitude > 0)
        {
            return Mathf.Atan2(direction.x, direction.z) * Mathf.Rad2Deg;
        }
        return rotation;
    }

	// Update is called once per frame
	void FixedUpdate ()
	{
        steering.Compute();
        steeringVelocity = steering.Velocity;
        float distance = (targetPos - Position).magnitude;

        if (IsArriving())
        {
            if (distance < MinDistToStop || hasArrived)
            {
                velocity = Vector3.zero;
                steeringVelocity = Vector3.zero;
                HasArrived = true;
            }
            else
            {
                Vector3 brakingVelocity = steeringVelocity.normalized * crtMaxSpeed * (distance / SlowingDistance);
                steeringVelocity = brakingVelocity - velocity;
            }
        }
        else
        {
            HasArrived = false;
        }

        // add steering force
        velocity += steeringVelocity;

        if (distance < MinDistToStop)
        {
            velocity = Vector3.zero;
        }
        else
        {
            rotation = GetOrientationFromDirection(velocity);
        }

        // truncate to max speed
        if (velocity.sqrMagnitude > GetSqrMaxSpeed())
		{
			velocity.Normalize();
            velocity *= crtMaxSpeed;
        }

        velocity.y = 0.0f;

        // Update position and rotation
        rb.velocity = velocity;
        transform.eulerAngles = Vector3.up * rotation;
	}

    // stop car movement at once / reset boost
    private void OnCollisionEnter(Collision collision)
    {
        HasArrived = true;
        velocity = Vector3.zero;
        TargetPos = transform.position;
        crtMaxSpeed = MaxSpeed;
    }

    void OnDrawGizmos()
	{
		Gizmos.color = Color.blue;
		Gizmos.DrawRay(transform.position, velocity);
        Gizmos.color = Color.green;
        Gizmos.DrawRay(transform.position, steeringVelocity * 5f);
    }

    public void StartBonus(int boostValue)
    {
        crtMaxSpeed = MaxSpeed * boostValue;
        if (boostCoroutine != null)
            StopCoroutine(boostCoroutine);
        boostCoroutine = StartCoroutine(BoostSequence());
    }

    private IEnumerator BoostSequence()
    {
        yield return new WaitForSeconds(boostDuration);
        boostCoroutine = StartCoroutine(DecreaseBoost());
    }

    private IEnumerator DecreaseBoost()
    {
        while(crtMaxSpeed > MaxSpeed)
        {
            crtMaxSpeed -= Time.deltaTime * boostDecreaseRate;
            yield return 0;
        }
        crtMaxSpeed = MaxSpeed;
        boostCoroutine = null;
    }
}
