using UnityEngine;
using System.Collections;

public class Car : MonoBehaviour
{
    Movement movement;
    int id = 0;
    public int ID { get { return id; } }

    // Use this for initialization
    void Awake()
    {
        movement = GetComponent<Movement>();
    }

    private void OnTriggerEnter(Collider other)
    {
        Bonus bonus = other.gameObject.GetComponent<Bonus>();
        if (bonus != null)
        {
            movement.StartBonus(bonus.BoostValue);
            Destroy(bonus.gameObject);
        }
    }

    public void SetTargetPos(Vector3 pos)
    {
        movement.TargetPos = pos;
    }
}
